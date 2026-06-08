#include "aiplayer.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <algorithm>
#include <QDebug>

Move AIPlayer::makeMove(const Board& board) {
    // Собираем все возможные ходы
    std::vector<Move> possibleMoves;
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(Position(r, c));
            if (p && p->color == color) {
                std::vector<Position> moves = p->getLegalMoves(board);
                for (const auto& to : moves) {
                    possibleMoves.push_back(Move(Position(r, c), to, p->getType()));
                }
            }
        }
    }
    
    // Фильтруем ходы, которые не оставляют короля под шахом
    std::vector<Move> safeMoves;
    for (const auto& move : possibleMoves) {
        // Создаем копию доски
        Board tempBoard = board.clone();
        
        // Выполняем ход на копии
        Piece* movingPiece = tempBoard.getPiece(move.from);
        if (movingPiece) {
            // Сохраняем информацию о взятии
            Piece* capturedPiece = tempBoard.getPiece(move.to);
            
            // Выполняем перемещение
            tempBoard.movePiece(move.from, move.to);
            
           
            if (movingPiece->getType() == PieceType::King && std::abs(move.to.col - move.from.col) == 2) {
                bool isKingside = move.to.col > move.from.col;
                int rookColFound = -1;
                
                if (isKingside) {
                    for(int c = move.from.col + 1; c < 8; ++c) {
                        if (tempBoard.getPiece(Position(move.from.row, c)) && 
                            tempBoard.getPiece(Position(move.from.row, c))->getType() == PieceType::Rook) {
                            rookColFound = c;
                            break;
                        }
                    }
                } else {
                    for(int c = move.from.col - 1; c >= 0; --c) {
                        if (tempBoard.getPiece(Position(move.from.row, c)) && 
                            tempBoard.getPiece(Position(move.from.row, c))->getType() == PieceType::Rook) {
                            rookColFound = c;
                            break;
                        }
                    }
                }
                
                if (rookColFound != -1) {
                    int targetRookCol = isKingside ? 5 : 3;
                    tempBoard.movePiece(Position(move.from.row, rookColFound), 
                                       Position(move.from.row, targetRookCol));
                }
            }
            
            // Обработка превращения пешки
            if (movingPiece->getType() == PieceType::Pawn && (move.to.row == 0 || move.to.row == 7)) {
                delete tempBoard.getPiece(move.to);
                tempBoard.cells[move.to.row][move.to.col] = new Queen(movingPiece->color, move.to);
            }
            
            // Проверяем, не под шахом ли король после хода
            Position kingPos = tempBoard.findKing(color);
            Color opponentColor = (color == Color::White) ? Color::Black : Color::White;
            
            if (!tempBoard.isSquareAttacked(kingPos, opponentColor)) {
                safeMoves.push_back(move);
            }
        }
    }
    
    // Если есть безопасные ходы, выбираем лучший
    if (!safeMoves.empty()) {
        // Оцениваем каждый ход и выбираем лучший
        Move bestMove = safeMoves[0];
        int bestScore = INT_MIN;
        
        for (const auto& move : safeMoves) {
            int score = evaluateMove(board, move);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        
        qDebug() << "AI chose move from" << bestMove.from.col << bestMove.from.row 
                 << "to" << bestMove.to.col << bestMove.to.row << "score:" << bestScore;
        
        return bestMove;
    }
    
    // Если нет безопасных ходов, возвращаем пустой ход (мат или пат)
    qDebug() << "AI has no safe moves!";
    return Move();
}

int AIPlayer::evaluateMove(const Board& board, const Move& move) const {
    int score = 0;
    
    Piece* piece = board.getPiece(move.from);
    Piece* target = board.getPiece(move.to);
    
    // 1. Приоритет взятия фигур
    if (target) {
        int victimValue = getPieceValue(target->getType());
        int attackerValue = getPieceValue(piece->getType());
        
        // Если берем более ценную фигуру менее ценной - большой бонус
        if (victimValue > attackerValue) {
            score += victimValue * 10;
        } else {
            score += victimValue;
        }
    }
    
    // 2. Бонус за превращение пешки
    if (piece->getType() == PieceType::Pawn && (move.to.row == 0 || move.to.row == 7)) {
        score += 800; // Ферзь ценен
    }
    
    // 3. Бонус за контроль центра
    if ((move.to.row == 3 || move.to.row == 4) && (move.to.col == 3 || move.to.col == 4)) {
        score += 30;
    }
    
    // 4. Бонус за продвижение пешки вперед
    if (piece->getType() == PieceType::Pawn) {
        int advancement = (color == Color::White) ? (7 - move.to.row) : move.to.row;
        score += advancement * 5;
    }
    
    // 5. Бонус за развитие в начале игры (вывод коней и слонов)
    if (piece->getType() == PieceType::Knight || piece->getType() == PieceType::Bishop) {
        // Если фигура уходит с начальной позиции
        bool isStartingPosition = (color == Color::White && move.from.row == 7) ||
                                  (color == Color::Black && move.from.row == 0);
        if (isStartingPosition) {
            score += 20;
        }
    }
    
    // 6. Штраф за ход королем в начале игры
    if (piece->getType() == PieceType::King) {
        bool hasMovedBefore = piece->hasMoved;
        if (!hasMovedBefore) {
            score -= 50; // Не хотим двигать короля без необходимости
        }
    }
    
    if (piece->getType() == PieceType::King && std::abs(move.to.col - move.from.col) == 2) {
        score += 60;
    }
    
    // 7. Штраф за ход на поле, которое атаковано противником
    Board tempBoard = board.clone();
    tempBoard.movePiece(move.from, move.to);
    Position newPiecePos = move.to;
    Color opponentColor = (color == Color::White) ? Color::Black : Color::White;
    if (tempBoard.isSquareAttacked(newPiecePos, opponentColor)) {
        score -= getPieceValue(piece->getType()) / 2;
    }
    
    return score;
}

int AIPlayer::getPieceValue(PieceType type) const {
    switch (type) {
        case PieceType::Pawn:   return 100;
        case PieceType::Knight: return 320;
        case PieceType::Bishop: return 330;
        case PieceType::Rook:   return 500;
        case PieceType::Queen:  return 900;
        case PieceType::King:   return 10000;
        default:                return 0;
    }
}