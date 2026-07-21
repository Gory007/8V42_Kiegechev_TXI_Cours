#include "gamewindow.h"
#include <QDebug>

GameWindow::GameWindow(QObject *parent)
    : QObject(parent), selectedPosition(-1, -1), isPieceSelected(false),
      isConnectionReady(false), myColor(Color::White), isAttemptingConnection(false)
{
    // Подключаем сигналы сетевых событий от GameController
    connect(&controller, &GameController::networkMoveReceived,
            this, &GameWindow::onNetworkMoveReceived);
    connect(&controller, &GameController::connectionEstablished,
            this, &GameWindow::onConnectionEstablished);

    // Подключаем сигнал ошибки от сетевого клиента
    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError);
    }
}

void GameWindow::startNewGame(int mode, int playerColor) {
    qDebug() << "=== GameWindow::startNewGame ===";
    qDebug() << "Mode:" << mode << "PlayerColor:" << playerColor;

    GameMode gameMode;
    if (mode == 0) gameMode = GameMode::PvP;
    else if (mode == 1) gameMode = GameMode::PvAI;
    else gameMode = GameMode::Network;

    Color color = (playerColor == 0) ? Color::White : Color::Black;

    controller.startNewGame(gameMode, color);
    clearSelection();

    // Сбрасываем состояние подключения для сетевой игры
    if (gameMode == GameMode::Network) {
        isConnectionReady = false;
        myColor = Color::White;
        isAttemptingConnection = false; // Сбрасываем флаг попытки подключения
    } else {
        isConnectionReady = true;
        myColor = color;
        isAttemptingConnection = false;
    }

    emit currentPlayerChanged();
    emit gameStateChanged();
    emit boardUpdated();
    emit networkGameChanged();
    emit canMakeMoveChanged();
    emit myColorChanged();
    emit isConnectingChanged();
}

void GameWindow::startServer(int port) {
    controller.startServer(port);
    isConnectionReady = false;
    myColor = Color::White;
    isAttemptingConnection = true; // ВАЖНО: устанавливаем флаг попытки подключения

    // Подключаем сигнал ошибки от клиента
    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError);
    }

    emit connectionStatusChanged("Сервер запущен, ожидание противника...");
    emit canMakeMoveChanged();
    emit myColorChanged();
    emit isConnectingChanged(); // ВАЖНО: уведомляем QML
}

void GameWindow::connectToServer(const QString& ip, int port) {
    controller.connectToServer(ip, port);
    isConnectionReady = false;
    myColor = Color::Black;
    isAttemptingConnection = true; // ВАЖНО: устанавливаем флаг попытки подключения

    // Подключаем сигнал ошибки от клиента
    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError);
    }

    emit connectionStatusChanged("Подключение к " + ip + ":" + QString::number(port) + "...");
    emit canMakeMoveChanged();
    emit myColorChanged();
    emit isConnectingChanged(); // ВАЖНО: уведомляем QML
}

void GameWindow::onConnectionEstablished() {
    isConnectionReady = true;
    isAttemptingConnection = false; // Сбрасываем флаг после успешного подключения
    emit connectionStatusChanged("Противник подключен! Игра началась.");
    emit boardUpdated();
    emit canMakeMoveChanged();
    emit isConnectingChanged();
}

void GameWindow::onConnectionError(const QString& error) {
    qWarning() << "Ошибка подключения:" << error;
    isConnectionReady = false;
    isAttemptingConnection = false; // Сбрасываем флаг после ошибки
    emit connectionStatusChanged("Ошибка: " + error);
    emit messageShown("Ошибка подключения: " + error);
    emit canMakeMoveChanged();
    emit isConnectingChanged(); // Разблокируем кнопки
}

void GameWindow::onNetworkMoveReceived(int fromRow, int fromCol, int toRow, int toCol) {
    qDebug() << "Получен ход по сети:" << fromRow << fromCol << "->" << toRow << toCol;

    GameState state = controller.checkGameState();
    if (state == GameState::Check) {
        emit messageShown("Шах!");
    } else if (state == GameState::Checkmate) {
        QString winner = (controller.getCurrentPlayer() == Color::White) ? "Чёрные" : "Белые";
        emit messageShown("Мат! Победили " + winner);
    } else if (state == GameState::Stalemate) {
        emit messageShown("Пат! Ничья");
    }

    emit currentPlayerChanged();
    emit gameStateChanged();
    emit boardUpdated();
    emit historyChanged();
    emit canMakeMoveChanged();
}

bool GameWindow::canMakeMove() const {
    if (controller.mode != GameMode::Network) {
        return true;
    }
    return isConnectionReady && (controller.getCurrentPlayer() == myColor);
}

void GameWindow::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!canMakeMove()) {
        if (controller.mode == GameMode::Network && !isConnectionReady) {
            emit messageShown("Ожидание подключения противника...");
        } else {
            emit messageShown("Сейчас ход противника!");
        }
        clearSelection();
        emit boardUpdated();
        return;
    }

    Position from(fromRow, fromCol);
    Position to(toRow, toCol);

    if (controller.handleMove(from, to)) {
        clearSelection();
        GameState state = controller.checkGameState();
        if (state == GameState::Check) {
            emit messageShown("Шах!");
        } else if (state == GameState::Checkmate) {
            QString winner = (controller.getCurrentPlayer() == Color::White) ? "Чёрные" : "Белые";
            emit messageShown("Мат! Победили " + winner);
        } else if (state == GameState::Stalemate) {
            emit messageShown("Пат! Ничья");
        }

        emit currentPlayerChanged();
        emit gameStateChanged();
        emit boardUpdated();
        emit historyChanged();
        emit canMakeMoveChanged();
    } else {
        emit messageShown("Некорректный ход!");
        clearSelection();
        emit boardUpdated();
    }
}

void GameWindow::selectPiece(int row, int col) {
    if (!canMakeMove()) {
        if (controller.mode == GameMode::Network && !isConnectionReady) {
            emit messageShown("Ожидание подключения противника...");
        } else {
            emit messageShown("Сейчас ход противника!");
        }
        return;
    }

    qDebug() << "selectPiece called:" << row << col;
    Piece* piece = controller.board.getPiece(Position(row, col));

    if (piece && piece->color == myColor) {
        if (isPieceSelected) {
            isPieceSelected = false;
        }
        selectedPosition = Position(row, col);
        isPieceSelected = true;
        qDebug() << "Piece selected at:" << row << col;
        emit boardUpdated();
    } else {
        qDebug() << "Cannot select piece at:" << row << col << "- wrong color or no piece";
        clearSelection();
    }
}

void GameWindow::clearSelection() {
    qDebug() << "clearSelection called";
    selectedPosition = Position(-1, -1);
    isPieceSelected = false;
    emit boardUpdated();
}

QVariantList GameWindow::getBoardState() const {
    QVariantList boardState;
    std::vector<Position> legalMoves;

    if (isPieceSelected && canMakeMove()) {
        Piece* piece = controller.board.getPiece(selectedPosition);
        if (piece && piece->color == myColor) {
            std::vector<Position> allMoves = piece->getLegalMoves(controller.board);
            for (const auto& to : allMoves) {
                Board tempBoard = controller.board.clone();
                tempBoard.movePiece(selectedPosition, to);
                Position kingPos = tempBoard.findKing(myColor);
                Color opponentColor = (myColor == Color::White) ? Color::Black : Color::White;
                if (!tempBoard.isSquareAttacked(kingPos, opponentColor)) {
                    legalMoves.push_back(to);
                }
            }
        } else {
            const_cast<GameWindow*>(this)->clearSelection();
        }
    }

    for (int r = 0; r < 8; ++r) {
        QVariantList row;
        for (int c = 0; c < 8; ++c) {
            Piece* piece = controller.board.getPiece(Position(r, c));
            QVariantMap cellData;
            cellData["row"] = r;
            cellData["col"] = c;
            cellData["hasPiece"] = false;
            cellData["color"] = "";
            cellData["type"] = "";
            cellData["isSelected"] = false;
            cellData["isLegalMove"] = false;

            if (piece) {
                cellData["hasPiece"] = true;
                cellData["color"] = (piece->color == Color::White) ? "white" : "black";
                switch (piece->getType()) {
                    case PieceType::King: cellData["type"] = "king"; break;
                    case PieceType::Queen: cellData["type"] = "queen"; break;
                    case PieceType::Rook: cellData["type"] = "rook"; break;
                    case PieceType::Bishop: cellData["type"] = "bishop"; break;
                    case PieceType::Knight: cellData["type"] = "knight"; break;
                    case PieceType::Pawn: cellData["type"] = "pawn"; break;
                    default: cellData["type"] = ""; break;
                }
            }

            if (isPieceSelected && selectedPosition.row == r && selectedPosition.col == c) {
                cellData["isSelected"] = true;
            }

            for (const auto& move : legalMoves) {
                if (move.row == r && move.col == c) {
                    cellData["isLegalMove"] = true;
                    break;
                }
            }
            row.append(cellData);
        }
        boardState.append(row);
    }
    return boardState;
}

QVariantList GameWindow::getMoveHistory() const {
    QVariantList history;
    auto moves = controller.history.getAll();
    int moveNumber = 1;
    for (size_t i = 0; i < moves.size(); i += 2) {
        QVariantMap movePair;
        movePair["number"] = moveNumber;
        movePair["white"] = QString::fromStdString(moves[i].toString());
        if (i + 1 < moves.size()) {
            movePair["black"] = QString::fromStdString(moves[i + 1].toString());
        } else {
            movePair["black"] = "";
        }
        history.append(movePair);
        moveNumber++;
    }
    return history;
}

void GameWindow::saveGame(const QString& filename) {
    controller.saveHistoryToFile(filename.toStdString());
    emit messageShown("Игра сохранена в " + filename);
}

Color GameWindow::getCurrentPlayer() const {
    return controller.getCurrentPlayer();
}

GameState GameWindow::getGameState() const {
    return controller.checkGameState();
}