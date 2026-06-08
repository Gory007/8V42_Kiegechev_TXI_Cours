#include "piece.h"
#include "board.h"

// Вспомогательная функция для проверки границ доски
static bool isInBounds(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

std::vector<Position> Pawn::getLegalMoves(const Board& board) const {
    std::vector<Position> moves;
    int dir = (color == Color::White) ? -1 : 1;
    int startRow = (color == Color::White) ? 6 : 1;

    // Ход вперед на 1
    int r1 = position.row + dir;
    if (isInBounds(r1, position.col) && board.isCellEmpty(Position(r1, position.col))) {
        moves.push_back(Position(r1, position.col));
        // Ход вперед на 2 с начальной позиции
        int r2 = position.row + 2 * dir;
        if (position.row == startRow && board.isCellEmpty(Position(r2, position.col))) {
            moves.push_back(Position(r2, position.col));
        }
    }

    // Взятие по диагонали
    for (int dc : {-1, 1}) {
        int c = position.col + dc;
        if (isInBounds(r1, c)) {
            Piece* target = board.getPiece(Position(r1, c));
            if (target && target->color != color) {
                moves.push_back(Position(r1, c));
            }
        }
    }
    return moves;
}

std::vector<Position> Knight::getLegalMoves(const Board& board) const {
    std::vector<Position> moves;
    int offsets[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for (auto& off : offsets) {
        int r = position.row + off[0];
        int c = position.col + off[1];
        if (isInBounds(r, c)) {
            Piece* target = board.getPiece(Position(r, c));
            if (!target || target->color != color) {
                moves.push_back(Position(r, c));
            }
        }
    }
    return moves;
}

std::vector<Position> King::getLegalMoves(const Board& board) const {
    std::vector<Position> moves;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int r = position.row + dr;
            int c = position.col + dc;
            if (isInBounds(r, c)) {
                Piece* target = board.getPiece(Position(r, c));
                if (!target || target->color != color) {
                    moves.push_back(Position(r, c));
                }
            }
        }
    }
    return moves;
}

// Вспомогательная функция для скользящих фигур (Ладья, Слон, Ферзь)
static std::vector<Position> getSlidingMoves(const Piece& piece, const Board& board, const std::vector<std::pair<int,int>>& directions) {
    std::vector<Position> moves;
    for (auto& dir : directions) {
        int r = piece.position.row + dir.first;
        int c = piece.position.col + dir.second;
        while (isInBounds(r, c)) {
            Piece* target = board.getPiece(Position(r, c));
            if (!target) {
                moves.push_back(Position(r, c));
            } else {
                if (target->color != piece.color) {
                    moves.push_back(Position(r, c));
                }
                break; // Блокировка фигурой
            }
            r += dir.first;
            c += dir.second;
        }
    }
    return moves;
}

std::vector<Position> Rook::getLegalMoves(const Board& board) const {
    return getSlidingMoves(*this, board, {{-1,0}, {1,0}, {0,-1}, {0,1}});
}

std::vector<Position> Bishop::getLegalMoves(const Board& board) const {
    return getSlidingMoves(*this, board, {{-1,-1}, {-1,1}, {1,-1}, {1,1}});
}

std::vector<Position> Queen::getLegalMoves(const Board& board) const {
    return getSlidingMoves(*this, board, {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}});
}