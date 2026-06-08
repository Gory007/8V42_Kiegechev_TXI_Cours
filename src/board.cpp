#include "board.h"
#include <algorithm>
#include <random>
#include <QDebug>
#include <iostream>

Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            cells[r][c] = nullptr;
}

Board::~Board() {
    clearBoard();
}

void Board::clearBoard() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            delete cells[r][c];
            cells[r][c] = nullptr;
        }
    }
}

Piece* Board::getPiece(Position pos) const {
    if (pos.row < 0 || pos.row > 7 || pos.col < 0 || pos.col > 7) return nullptr;
    return cells[pos.row][pos.col];
}

void Board::movePiece(Position from, Position to) {
    Piece* piece = getPiece(from);
    if (piece) {
        delete cells[to.row][to.col]; 
        cells[to.row][to.col] = piece;
        cells[from.row][from.col] = nullptr;
        piece->position = to;
        piece->hasMoved = true;
    }
}

bool Board::isCellEmpty(Position pos) const {
    return getPiece(pos) == nullptr;
}

Board Board::clone() const {
    Board newBoard;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (cells[r][c]) {
                Piece* p = cells[r][c];
                Piece* newPiece = nullptr;
                // Создаем копию фигуры того же типа
                switch (p->getType()) {
                    case PieceType::Pawn:   newPiece = new Pawn(p->color, Position(r, c)); break;
                    case PieceType::Knight: newPiece = new Knight(p->color, Position(r, c)); break;
                    case PieceType::Bishop: newPiece = new Bishop(p->color, Position(r, c)); break;
                    case PieceType::Rook:   newPiece = new Rook(p->color, Position(r, c)); break;
                    case PieceType::Queen:  newPiece = new Queen(p->color, Position(r, c)); break;
                    case PieceType::King:   newPiece = new King(p->color, Position(r, c)); break;
                }
                newBoard.cells[r][c] = newPiece;
            }
        }
    }
    return newBoard;
}

void Board::generateInitialPosition() {
    qDebug() << "=== Board::generateInitialPosition ===";
    clearBoard();
    std::mt19937 rng(std::random_device{}());
    
    qDebug() << "Placing back rank pieces...";

    // Лямбда-функция для расстановки фигур на одной горизонтали
    auto placeBackRank = [&](int row, Color color) {
        std::vector<int> available = {0, 1, 2, 3, 4, 5, 6, 7};

        // 1. Слоны на полях разного цвета (четные и нечетные индексы)
        std::vector<int> darkSquares = {0, 2, 4, 6};
        std::vector<int> lightSquares = {1, 3, 5, 7};

        int darkIdx = darkSquares[rng() % darkSquares.size()];
        int lightIdx = lightSquares[rng() % lightSquares.size()];

        cells[row][darkIdx] = new Bishop(color, Position(row, darkIdx));
        cells[row][lightIdx] = new Bishop(color, Position(row, lightIdx));

        // Удаляем занятые клетки из списка доступных
        available.erase(std::remove(available.begin(), available.end(), darkIdx), available.end());
        available.erase(std::remove(available.begin(), available.end(), lightIdx), available.end());

        // 2. Ферзь
        int qIdx = available[rng() % available.size()];
        cells[row][qIdx] = new Queen(color, Position(row, qIdx));
        available.erase(std::remove(available.begin(), available.end(), qIdx), available.end());

        // 3. Два коня
        int n1Idx = available[rng() % available.size()];
        cells[row][n1Idx] = new Knight(color, Position(row, n1Idx));
        available.erase(std::remove(available.begin(), available.end(), n1Idx), available.end());

        int n2Idx = available[rng() % available.size()];
        cells[row][n2Idx] = new Knight(color, Position(row, n2Idx));
        available.erase(std::remove(available.begin(), available.end(), n2Idx), available.end());

        // 4. Ладьи и Король
        cells[row][available[0]] = new Rook(color, Position(row, available[0]));
        cells[row][available[1]] = new King(color, Position(row, available[1]));
        cells[row][available[2]] = new Rook(color, Position(row, available[2]));
    };

    // Белые фигуры внизу (ряд 7), черные вверху (ряд 0)
    placeBackRank(7, Color::White);
    placeBackRank(0, Color::Black);

    // Пешки
    for (int c = 0; c < 8; ++c) {
        cells[6][c] = new Pawn(Color::White, Position(6, c));
        cells[1][c] = new Pawn(Color::Black, Position(1, c));
    }

    qDebug() << "generateInitialPosition completed - pieces placed";
    
    // Проверка - выводим что на доске
    int pieceCount = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (cells[r][c]) pieceCount++;
        }
    }
    qDebug() << "Total pieces on board:" << pieceCount;
}

Position Board::findKing(Color color) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (cells[r][c] && cells[r][c]->getType() == PieceType::King && cells[r][c]->color == color) {
                return Position(r, c);
            }
        }
    }
    return Position(-1, -1); // Король не найден (ошибка)
}

bool Board::isSquareAttacked(Position pos, Color byColor) const {
    // Проходим по всем фигурам противника и смотрим, могут ли они пойти на pos
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = cells[r][c];
            if (p && p->color == byColor) {
                std::vector<Position> moves = p->getLegalMoves(*this);
                for (const auto& m : moves) {
                    if (m == pos) return true;
                }
            }
        }
    }
    return false;
}