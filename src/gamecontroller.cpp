#include "gamecontroller.h"
#include <QDebug>
#include <fstream>

GameController::GameController()
    : aiPlayer(nullptr)
    , currentPlayer(Color::White)
    , mode(GameMode::PvP)
    , networkManager(new NetworkManager())
{
    connect(networkManager, &NetworkManager::moveReceived,
            this, &GameController::onNetworkMoveReceived);
    connect(networkManager, &NetworkManager::clientConnected,
            this, &GameController::connectionEstablished);
    connect(networkManager, &NetworkManager::connectedToServer,
            this, &GameController::connectionEstablished);
    
    // Подключаем новый сигнал для получения начальной позиции от сервера
    connect(networkManager, &NetworkManager::initPositionReceived,
            this, &GameController::onNetworkInitReceived);
}

GameController::~GameController() {
    delete aiPlayer;
    delete networkManager;
}

void GameController::startNewGame(GameMode gameMode, Color playerColor) {
    qDebug() << "=== GameController::startNewGame ===";
    qDebug() << "Mode:" << (int)gameMode << "PlayerColor:" << (int)playerColor;

    mode = gameMode;
    currentPlayer = Color::White;
    history.clear();

    if (mode == GameMode::Network) {
        // В сетевой игре доска инициализируется ТОЛЬКО сервером.
        // Клиент ждет получения позиции от сервера, чтобы избежать рассинхрона.
        board.clearBoard();
    } else {
        // Для локальных режимов генерируем позицию сразу
        board.generateInitialPosition();
    }

    if (mode == GameMode::PvAI) {
        aiPlayer = new AIPlayer(playerColor == Color::White ? Color::Black : Color::White);
        
        // ИСПРАВЛЕНИЕ БАГА: Если ИИ играет белыми, он должен сделать первый ход автоматически
        if (aiPlayer->color == Color::White) {
            QTimer::singleShot(500, this, &GameController::makeAiMove);
        }
    } else {
        delete aiPlayer;
        aiPlayer = nullptr;
    }

    qDebug() << "startNewGame finished";
    emit boardUpdated();
}

void GameController::makeAiMove() {
    if (mode != GameMode::PvAI || !aiPlayer || currentPlayer != aiPlayer->color) {
        return;
    }

    Move aiMove = aiPlayer->makeMove(board);
    
    // ПРОВЕРКА: валиден ли ход
    if (aiMove.from.row >= 0 && aiMove.from.row < 8 && aiMove.from.col >= 0 && aiMove.from.col < 8 &&
        aiMove.to.row >= 0 && aiMove.to.row < 8 && aiMove.to.col >= 0 && aiMove.to.col < 8) {
        
        if (validateMove(aiMove.from, aiMove.to)) {
            handleMove(aiMove.from, aiMove.to);
            return;
        }
    }

    qDebug() << "AI generated an invalid move, trying to find any valid move...";
    // Fallback: найти любой валидный ход
    for (int r = 0; r < 8 && currentPlayer == aiPlayer->color; ++r) {
        for (int c = 0; c < 8 && currentPlayer == aiPlayer->color; ++c) {
            Piece* p = board.getPiece(Position(r, c));
            if (p && p->color == currentPlayer) {
                std::vector<Position> moves = p->getLegalMoves(board);
                for (const auto& to : moves) {
                    if (validateMove(Position(r, c), to)) {
                        handleMove(Position(r, c), to);
                        return;
                    }
                }
            }
        }
    }
    
    qDebug() << "AI could not find any valid move!";
}

bool GameController::validateMove(Position from, Position to) const {
    Piece* piece = board.getPiece(from);
    if (!piece || piece->color != currentPlayer) return false;

    std::vector<Position> legalMoves = piece->getLegalMoves(board);
    bool isMoveGeometricallyValid = false;
    for (const auto& pos : legalMoves) {
        if (pos == to) {
            isMoveGeometricallyValid = true;
            break;
        }
    }
    if (!isMoveGeometricallyValid) return false;

    Board tempBoard = board.clone();
    tempBoard.movePiece(from, to);

    Position myKingPos = tempBoard.findKing(currentPlayer);
    Color opponentColor = (currentPlayer == Color::White) ? Color::Black : Color::White;

    if (tempBoard.isSquareAttacked(myKingPos, opponentColor)) {
        return false;
    }

    return true;
}

bool GameController::handleMove(Position from, Position to) {
    if (!validateMove(from, to)) return false;

    Piece* movingPiece = board.getPiece(from);
    Piece* capturedPiece = board.getPiece(to);

    bool isCastling = (movingPiece->getType() == PieceType::King && std::abs(to.col - from.col) == 2);
    bool isPromotion = (movingPiece->getType() == PieceType::Pawn && (to.row == 0 || to.row == 7));

    Move move(from, to, movingPiece->getType(),
              capturedPiece ? capturedPiece->getType() : PieceType::Pawn,
              isCastling, isPromotion);

    board.movePiece(from, to);

    if (isCastling) {
        bool isKingside = to.col > from.col;
        int rookColFound = -1;

        if (isKingside) {
            for(int c = from.col + 1; c < 8; ++c) {
                if (board.getPiece(Position(from.row, c)) &&
                    board.getPiece(Position(from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c;
                    break;
                }
            }
        } else {
            for(int c = from.col - 1; c >= 0; --c) {
                if (board.getPiece(Position(from.row, c)) &&
                    board.getPiece(Position(from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c;
                    break;
                }
            }
        }

        if (rookColFound != -1) {
            int targetRookCol = isKingside ? 5 : 3;
            board.movePiece(Position(from.row, rookColFound), Position(from.row, targetRookCol));
        }
    }

    if (isPromotion) {
        delete board.getPiece(to);
        board.cells[to.row][to.col] = new Queen(currentPlayer, to);
        board.cells[to.row][to.col]->hasMoved = true;
    }

    history.addMove(move);

    if (mode == GameMode::Network) {
        networkManager->sendMove(move);
    }

    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
    emit boardUpdated();

    // Если режим PvAI и сейчас ход ИИ, делаем его автоматически с небольшой задержкой для обновления UI
    if (mode == GameMode::PvAI && aiPlayer && currentPlayer == aiPlayer->color) {
        QTimer::singleShot(500, this, &GameController::makeAiMove);
    }

    return true;
}

void GameController::handleNetworkMove(const Move& move) {
    board.movePiece(move.from, move.to);

    if (move.isCastling) {
        bool isKingside = move.to.col > move.from.col;
        int rookColFound = -1;

        if (isKingside) {
            for(int c = move.from.col + 1; c < 8; ++c) {
                if (board.getPiece(Position(move.from.row, c)) &&
                    board.getPiece(Position(move.from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c;
                    break;
                }
            }
        } else {
            for(int c = move.from.col - 1; c >= 0; --c) {
                if (board.getPiece(Position(move.from.row, c)) &&
                    board.getPiece(Position(move.from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c;
                    break;
                }
            }
        }

        if (rookColFound != -1) {
            int targetRookCol = isKingside ? 5 : 3;
            board.movePiece(Position(move.from.row, rookColFound), Position(move.from.row, targetRookCol));
        }
    }

    if (move.isPromotion) {
        delete board.getPiece(move.to);
        // Цвет фигуры должен быть противоположен текущему игроку, так как ход уже был сделан противником
        Color opponentColor = (currentPlayer == Color::White) ? Color::Black : Color::White;
        board.cells[move.to.row][move.to.col] = new Queen(opponentColor, move.to);
        board.cells[move.to.row][move.to.col]->hasMoved = true;
    }

    history.addMove(move);
    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
    emit boardUpdated();
}

void GameController::handleNetworkInit(const std::vector<int>& backRank) {
    board.setInitialPosition(backRank);
    currentPlayer = Color::White;
    emit boardUpdated();
}

void GameController::onNetworkMoveReceived(const Move& move) {
    handleNetworkMove(move);
    emit networkMoveReceived(move.from.row, move.from.col, move.to.row, move.to.col);
}

void GameController::onNetworkInitReceived(const std::vector<int>& backRank) {
    handleNetworkInit(backRank);
}

void GameController::startServer(quint16 port) {
    mode = GameMode::Network;
    // Генерируем позицию на сервере и сохраняем её для отправки клиенту при подключении
    std::vector<int> backRank = board.generateInitialPosition();
    networkManager->setPendingInitPosition(backRank);
    networkManager->createServer(port);
}

void GameController::connectToServer(const QString& ip, quint16 port) {
    mode = GameMode::Network;
    networkManager->connectToHost(ip, port);
}

GameState GameController::checkGameState() const {
    Position kingPos = board.findKing(currentPlayer);
    Color opponentColor = (currentPlayer == Color::White) ? Color::Black : Color::White;
    bool isKingInCheck = board.isSquareAttacked(kingPos, opponentColor);

    bool hasLegalMoves = false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(Position(r, c));
            if (p && p->color == currentPlayer) {
                std::vector<Position> moves = p->getLegalMoves(board);
                for (const auto& to : moves) {
                    if (validateMove(Position(r, c), to)) {
                        hasLegalMoves = true;
                        break;
                    }
                }
            }
            if (hasLegalMoves) break;
        }
        if (hasLegalMoves) break;
    }

    if (!hasLegalMoves) {
        return isKingInCheck ? GameState::Checkmate : GameState::Stalemate;
    }

    return isKingInCheck ? GameState::Check : GameState::Playing;
}

void GameController::saveHistoryToFile(const std::string& path) {
    std::ofstream file(path);
    if (file.is_open()) {
        for (const auto& move : history.getAll()) {
            file << move.toString() << "\n";
        }
        file.close();
    }
}