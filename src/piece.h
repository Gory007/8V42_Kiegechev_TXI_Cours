#ifndef PIECE_H
#define PIECE_H

#include "position.h"
#include <vector>

enum class Color {
    White,
    Black
};

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

class Board; // Предварительное объявление, чтобы избежать циклических зависимостей

class Piece {
public:
    Color color;
    Position position;
    bool hasMoved;

    Piece(Color color, Position position) : color(color), position(position), hasMoved(false) {}
    virtual ~Piece() = default;

    // Чисто виртуальный метод для получения легальных ходов
    virtual std::vector<Position> getLegalMoves(const Board& board) const = 0;
    
    // Метод для получения типа фигуры
    virtual PieceType getType() const = 0;
};

class Pawn : public Piece {
public:
    Pawn(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::Pawn; }
};

class Knight : public Piece {
public:
    Knight(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::Knight; }
};

class King : public Piece {
public:
    King(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::King; }
};

class Queen : public Piece {
public:
    Queen(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::Queen; }
};

class Rook : public Piece {
public:
    Rook(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::Rook; }
};

class Bishop : public Piece {
public:
    Bishop(Color color, Position position) : Piece(color, position) {}
    std::vector<Position> getLegalMoves(const Board& board) const override;
    PieceType getType() const override { return PieceType::Bishop; }
};

#endif // PIECE_H