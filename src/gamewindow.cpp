#include "gamewindow.h"
#include <QDebug>
#include <QTimer>

GameWindow::GameWindow(QObject *parent)
    : QObject(parent), selectedPosition(-1, -1), isPieceSelected(false),
      isConnectionReady(false), myColor(Color::White), isAttemptingConnection(false)
{
    connect(&controller, &GameController::networkMoveReceived,
            this, &GameWindow::onNetworkMoveReceived);
    connect(&controller, &GameController::connectionEstablished,
            this, &GameWindow::onConnectionEstablished);
    connect(&controller, &GameController::boardUpdated,
            this, &GameWindow::onBoardUpdated);
    
    // НОВОЕ: Подключаем сигнал о завершении сетевой игры (разрыв соединения)
    connect(&controller, &GameController::networkGameEnded,
            this, &GameWindow::onNetworkGameEnded);

    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError, Qt::UniqueConnection);
    }
}

void GameWindow::startNewGame(int mode, int playerColor) {
    GameMode gameMode = (mode == 0) ? GameMode::PvP : ((mode == 1) ? GameMode::PvAI : GameMode::Network);
    Color color = (playerColor == 0) ? Color::White : Color::Black;

    // НОВОЕ: Если мы переключаемся из сетевого режима в локальный, 
    // обязательно останавливаем сеть, чтобы освободить порты и сокеты.
    if (controller.mode == GameMode::Network && gameMode != GameMode::Network) {
        controller.networkManager->stopNetwork();
    }

    controller.startNewGame(gameMode, color);
    clearSelection();

    if (gameMode == GameMode::Network) {
        isConnectionReady = false;
        myColor = Color::White;
        isAttemptingConnection = false;
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
    emit connectionStatusChanged("");

    if (gameMode == GameMode::PvAI && controller.getCurrentPlayer() != color) {
        QTimer::singleShot(500, &controller, &GameController::makeAiMove);
    }
}

void GameWindow::startServer(int port) {
    if (isAttemptingConnection || isConnectionReady) {
        emit messageShown("Сервер уже создается или игра уже началась");
        return;
    }

    controller.startServer(port);
    isConnectionReady = false;
    myColor = Color::White;
    isAttemptingConnection = true;

    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError, Qt::UniqueConnection);
    }

    emit connectionStatusChanged("Сервер запущен, ожидание противника...");
    emit canMakeMoveChanged();
    emit myColorChanged();
    emit isConnectingChanged();
}

void GameWindow::connectToServer(const QString& ip, int port) {
    if (isAttemptingConnection || isConnectionReady) {
        emit messageShown("Уже идет попытка подключения или игра уже началась");
        return;
    }

    controller.connectToServer(ip, port);
    isConnectionReady = false;
    myColor = Color::Black;
    isAttemptingConnection = true;

    if (controller.networkManager && controller.networkManager->client()) {
        connect(controller.networkManager->client(), &NetworkClient::connectionError,
                this, &GameWindow::onConnectionError, Qt::UniqueConnection);
    }

    emit connectionStatusChanged("Подключение к " + ip + ":" + QString::number(port) + "...");
    emit canMakeMoveChanged();
    emit myColorChanged();
    emit isConnectingChanged();
}

void GameWindow::onConnectionEstablished() {
    isConnectionReady = true;
    isAttemptingConnection = false;

    emit connectionStatusChanged("Противник подключен! Игра началась.");
    emit boardUpdated();
    emit canMakeMoveChanged();
    emit isConnectingChanged();
}

void GameWindow::onConnectionError(const QString& error) {
    isConnectionReady = false;
    isAttemptingConnection = false;

    emit connectionStatusChanged("Ошибка: " + error);
    emit messageShown("Ошибка подключения: " + error);
    emit canMakeMoveChanged();
    emit isConnectingChanged();
}

// НОВАЯ РЕАЛИЗАЦИЯ: Обработка разрыва соединения
void GameWindow::onNetworkGameEnded(const QString& reason) {
    qDebug() << "UI: Network game ended -" << reason;
    
    // Сбрасываем флаги соединения, чтобы разблокировать интерфейс
    isConnectionReady = false;
    isAttemptingConnection = false;
    
    // Показываем сообщение пользователю
    emit messageShown(reason);
    emit connectionStatusChanged("Игра завершена: " + reason);
    
    // Обновляем состояние UI, чтобы можно было сразу начать новую партию
    emit canMakeMoveChanged();
    emit isConnectingChanged();
    emit boardUpdated();
    emit networkGameChanged();
}

void GameWindow::onNetworkMoveReceived(int fromRow, int fromCol, int toRow, int toCol) {
    GameState state = controller.checkGameState();

    if (state == GameState::Check) emit messageShown("Шах!");
    else if (state == GameState::Checkmate) {
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

void GameWindow::onBoardUpdated() {
    emit boardUpdated();
    emit currentPlayerChanged();
    emit canMakeMoveChanged();
    emit gameStateChanged();
    emit historyChanged();
}

bool GameWindow::canMakeMove() const {
    if (controller.mode == GameMode::PvP) return true;
    return isConnectionReady && (controller.getCurrentPlayer() == myColor);
}

void GameWindow::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!canMakeMove()) {
        emit messageShown(controller.mode == GameMode::Network && !isConnectionReady ?
                            "Ожидание подключения противника..." : "Сейчас ход противника!");
        clearSelection();
        return;
    }

    if (controller.handleMove(Position(fromRow, fromCol), Position(toRow, toCol))) {
        clearSelection();

        GameState state = controller.checkGameState();
        if (state == GameState::Check) emit messageShown("Шах!");
        else if (state == GameState::Checkmate) {
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
    }
}

void GameWindow::selectPiece(int row, int col) {
    if (!canMakeMove()) {
        emit messageShown(controller.mode == GameMode::Network && !isConnectionReady ?
                            "Ожидание подключения противника..." : "Сейчас ход противника!");
        return;
    }

    Piece* piece = controller.board.getPiece(Position(row, col));
    if (piece && piece->color == controller.getCurrentPlayer()) {
        selectedPosition = Position(row, col);
        isPieceSelected = true;
        emit boardUpdated();
    } else {
        clearSelection();
    }
}

void GameWindow::clearSelection() {
    selectedPosition = Position(-1, -1);
    isPieceSelected = false;
    emit boardUpdated();
}

QVariantList GameWindow::getBoardState() const {
    QVariantList boardState;
    std::vector<Position> legalMoves;

    if (isPieceSelected && canMakeMove()) {
        Piece* piece = controller.board.getPiece(selectedPosition);
        if (piece && piece->color == controller.getCurrentPlayer()) {
            std::vector<Position> allMoves = piece->getLegalMoves(controller.board);
            for (const auto& to : allMoves) {
                Board tempBoard = controller.board.clone();
                tempBoard.movePiece(selectedPosition, to);

                Position kingPos = tempBoard.findKing(piece->color);
                Color opponentColor = (piece->color == Color::White) ? Color::Black : Color::White;

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
        movePair["black"] = (i + 1 < moves.size()) ? QString::fromStdString(moves[i + 1].toString()) : "";
        history.append(movePair);
        moveNumber++;
    }
    return history;
}

void GameWindow::saveGame(const QString& filename) {
    controller.saveHistoryToFile(filename.toStdString());
    emit messageShown("Игра сохранена в " + filename);
}

Color GameWindow::getCurrentPlayer() const { return controller.getCurrentPlayer(); }
GameState GameWindow::getGameState() const { return controller.checkGameState(); }