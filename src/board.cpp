#include "board.h"
#include <algorithm>
#include <random>
#include <QDebug>
#include <iostream>
#include <chrono>

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
    qDebug() << "=== Board::generateInitialPosition (Chess960) ===";
    clearBoard();
    
    // Генерируем случайную позицию для белых
    std::vector<int> whiteBackRank = generateRandomChess960Position();
    
    // Расставляем белые фигуры (ряд 7)
    for (int col = 0; col < 8; ++col) {
        if (whiteBackRank[col] == -1) {
            qDebug() << "ERROR: Invalid piece type at column" << col;
            continue;
        }
        
        PieceType type = static_cast<PieceType>(whiteBackRank[col]);
        switch (type) {
            case PieceType::King:   cells[7][col] = new King(Color::White, Position(7, col)); break;
            case PieceType::Queen:  cells[7][col] = new Queen(Color::White, Position(7, col)); break;
            case PieceType::Rook:   cells[7][col] = new Rook(Color::White, Position(7, col)); break;
            case PieceType::Bishop: cells[7][col] = new Bishop(Color::White, Position(7, col)); break;
            case PieceType::Knight: cells[7][col] = new Knight(Color::White, Position(7, col)); break;
            default: 
                qDebug() << "ERROR: Unknown piece type" << whiteBackRank[col];
                break;
        }
    }
    
    // Расставляем черные фигуры (ряд 0) - зеркальное отражение
    for (int col = 0; col < 8; ++col) {
        if (whiteBackRank[col] == -1) {
            qDebug() << "ERROR: Invalid piece type at column" << col;
            continue;
        }
        
        PieceType type = static_cast<PieceType>(whiteBackRank[col]);
        switch (type) {
            case PieceType::King:   cells[0][col] = new King(Color::Black, Position(0, col)); break;
            case PieceType::Queen:  cells[0][col] = new Queen(Color::Black, Position(0, col)); break;
            case PieceType::Rook:   cells[0][col] = new Rook(Color::Black, Position(0, col)); break;
            case PieceType::Bishop: cells[0][col] = new Bishop(Color::Black, Position(0, col)); break;
            case PieceType::Knight: cells[0][col] = new Knight(Color::Black, Position(0, col)); break;
            default: break;
        }
    }
    
    // Пешки
    for (int c = 0; c < 8; ++c) {
        cells[6][c] = new Pawn(Color::White, Position(6, c));
        cells[1][c] = new Pawn(Color::Black, Position(1, c));
    }
    
    qDebug() << "generateInitialPosition completed - symmetric Chess960 position";
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

std::vector<int> Board::generateRandomChess960Position() {
    std::random_device rd;
    std::mt19937 rng(rd());
    
    // Все возможные позиции для белых в Chess960
    // Используем метод перебора до валидной позиции
    std::vector<int> position(8);
    bool validPosition = false;
    int attempts = 0;
    
    while (!validPosition && attempts < 1000) {
        attempts++;
        
        // Заполняем случайными типами фигур
        std::vector<int> pieces = {
            static_cast<int>(PieceType::Rook),
            static_cast<int>(PieceType::Knight),
            static_cast<int>(PieceType::Bishop),
            static_cast<int>(PieceType::Queen),
            static_cast<int>(PieceType::King),
            static_cast<int>(PieceType::Bishop),
            static_cast<int>(PieceType::Knight),
            static_cast<int>(PieceType::Rook)
        };
        
        // Перемешиваем
        std::shuffle(pieces.begin(), pieces.end(), rng);
        
        // Копируем в позицию
        for (int i = 0; i < 8; ++i) {
            position[i] = pieces[i];
        }
        
        // Проверяем правила Chess960
        // 1. Слоны на разных цветах
        int bishopPos1 = -1, bishopPos2 = -1;
        for (int i = 0; i < 8; ++i) {
            if (position[i] == static_cast<int>(PieceType::Bishop)) {
                if (bishopPos1 == -1) bishopPos1 = i;
                else bishopPos2 = i;
            }
        }
        
        bool bishopsOk = ((bishopPos1 + bishopPos2) % 2 != 0);
        
        // 2. Король между ладьями
        int kingPos = -1;
        int rookPos1 = -1, rookPos2 = -1;
        for (int i = 0; i < 8; ++i) {
            if (position[i] == static_cast<int>(PieceType::King)) kingPos = i;
            if (position[i] == static_cast<int>(PieceType::Rook)) {
                if (rookPos1 == -1) rookPos1 = i;
                else rookPos2 = i;
            }
        }
        
        bool kingOk = (rookPos1 < kingPos && kingPos < rookPos2);
        
        if (bishopsOk && kingOk) {
            validPosition = true;
            qDebug() << "Valid Chess960 position found after" << attempts << "attempts";
        }
    }
    
    if (!validPosition) {
        qDebug() << "Using fallback position";
        // Стандартная расстановка
        position = {
            static_cast<int>(PieceType::Rook),
            static_cast<int>(PieceType::Knight),
            static_cast<int>(PieceType::Bishop),
            static_cast<int>(PieceType::Queen),
            static_cast<int>(PieceType::King),
            static_cast<int>(PieceType::Bishop),
            static_cast<int>(PieceType::Knight),
            static_cast<int>(PieceType::Rook)
        };
    }
    
    // Вывод позиции для отладки
    qDebug() << "Generated Chess960 position:";
    for (int col = 0; col < 8; ++col) {
        QString pieceName;
        switch (position[col]) {
            case 0: pieceName = "King"; break;
            case 1: pieceName = "Queen"; break;
            case 2: pieceName = "Rook"; break;
            case 3: pieceName = "Bishop"; break;
            case 4: pieceName = "Knight"; break;
            default: pieceName = "Unknown"; break;
        }
        qDebug() << "  Col" << col << ":" << pieceName;
    }
    
    return position;
}