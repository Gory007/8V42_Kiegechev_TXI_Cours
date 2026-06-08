#ifndef MOVE_H
#define MOVE_H

#include "position.h"
#include "piece.h"
#include <string>

class Move {
public:
    Position from;
    Position to;
    PieceType piece; // В UML указано PieceType*, но для простоты и безопасности памяти лучше хранить тип или умный указатель. Оставим PieceType.
    PieceType captured;
    bool isCastling;
    bool isPromotion;

    Move();
    Move(Position from, Position to, PieceType piece, PieceType captured = PieceType::Pawn, bool isCastling = false, bool isPromotion = false);

    std::string toString() const;
};

#endif // MOVE_H