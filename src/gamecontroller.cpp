#include "gamecontroller.h"
#include <fstream>
#include <iostream>
#include <QDebug>

GameController::GameController() : aiPlayer(nullptr), currentPlayer(Color::White), mode(GameMode::PvP) {}

GameController::~GameController() {
    delete aiPlayer;
}

void GameController::startNewGame(GameMode gameMode, Color playerColor) {
    qDebug() << "=== GameController::startNewGame ===";
    qDebug() << "Mode:" << (int)gameMode << "PlayerColor:" << (int)playerColor;
    
    mode = gameMode;
    currentPlayer = Color::White;
    
    qDebug() << "Calling generateInitialPosition...";
    board.generateInitialPosition();
    qDebug() << "generateInitialPosition completed";
    
    history.clear();
    
    if (mode == GameMode::PvAI) {
        aiPlayer = new AIPlayer(playerColor == Color::White ? Color::Black : Color::White);
    } else {
        delete aiPlayer;
        aiPlayer = nullptr;
    }
    
    qDebug() << "startNewGame finished";
}

bool GameController::validateMove(Position from, Position to) const{
    Piece* piece = board.getPiece(from);
    if (!piece || piece->color != currentPlayer) return false;

    // 1. Проверка геометрической возможности хода
    std::vector<Position> legalMoves = piece->getLegalMoves(board);
    bool isMoveGeometricallyValid = false;
    for (const auto& pos : legalMoves) {
        if (pos == to) {
            isMoveGeometricallyValid = true;
            break;
        }
    }
    if (!isMoveGeometricallyValid) return false;

    // 2. Проверка, не оставляет ли ход короля под шахом
    Board tempBoard = board.clone();
    tempBoard.movePiece(from, to);
    
    Position myKingPos = tempBoard.findKing(currentPlayer);
    Color opponentColor = (currentPlayer == Color::White) ? Color::Black : Color::White;
    
    if (tempBoard.isSquareAttacked(myKingPos, opponentColor)) {
        return false; // Ход недопустим, так как король остается под шахом
    }

    return true;
}

bool GameController::handleMove(Position from, Position to) {
    if (!validateMove(from, to)) return false;

    Piece* movingPiece = board.getPiece(from);
    Piece* capturedPiece = board.getPiece(to);
    
    
    bool isCastling = (movingPiece->getType() == PieceType::King && std::abs(to.col - from.col) == 2);

    Move move(from, to, movingPiece->getType(), 
              capturedPiece ? capturedPiece->getType() : PieceType::Pawn, isCastling);
              
    // Выполняем ход фигуры
    board.movePiece(from, to);

    
    if (isCastling) {
        bool isKingside = to.col > from.col;
        int rookColFound = -1;
        

        if (isKingside) {
            for(int c = from.col + 1; c < 8; ++c) {
                if (board.getPiece(Position(from.row, c)) && board.getPiece(Position(from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c; break;
                }
            }
        } else {
            for(int c = from.col - 1; c >= 0; --c) {
                if (board.getPiece(Position(from.row, c)) && board.getPiece(Position(from.row, c))->getType() == PieceType::Rook) {
                    rookColFound = c; break;
                }
            }
        }
        
        if (rookColFound != -1) {
            int targetRookCol = isKingside ? 5 : 3; 
            board.movePiece(Position(from.row, rookColFound), Position(from.row, targetRookCol));
        }
    }

    // Превращение пешки (автоматически в ферзя для упрощения)
    if (movingPiece->getType() == PieceType::Pawn && (to.row == 0 || to.row == 7)) {
        delete board.getPiece(to); // Удаляем пешку
        board.cells[to.row][to.col] = new Queen(movingPiece->color, to);
        board.cells[to.row][to.col]->hasMoved = true;
    }

    history.addMove(move);

    // Смена хода
    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;

    // Ход ИИ
    if (mode == GameMode::PvAI && aiPlayer && currentPlayer == aiPlayer->color) {
        Move aiMove = aiPlayer->makeMove(board);
        if (validateMove(aiMove.from, aiMove.to)) {
             // Рекурсивно вызываем handleMove для ИИ, чтобы сработала вся логика
             handleMove(aiMove.from, aiMove.to);
             return true; 
        }
    }

    return true;
}

GameState GameController::checkGameState() const{
    Position kingPos = board.findKing(currentPlayer);
    Color opponentColor = (currentPlayer == Color::White) ? Color::Black : Color::White;
    bool isKingInCheck = board.isSquareAttacked(kingPos, opponentColor);

    // Проверяем, есть ли хотя бы один легальный ход
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