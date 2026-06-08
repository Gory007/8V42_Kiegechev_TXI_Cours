#include "move.h"
#include <sstream>

Move::Move() 
    : from(), to(), piece(PieceType::Pawn), captured(PieceType::Pawn), 
      isCastling(false), isPromotion(false) {}

Move::Move(Position from, Position to, PieceType piece, PieceType captured, bool isCastling, bool isPromotion)
    : from(from), to(to), piece(piece), captured(captured), 
      isCastling(isCastling), isPromotion(isPromotion) {}

std::string Move::toString() const {
    std::ostringstream oss;
    oss << static_cast<char>('a' + from.col) << (8 - from.row) 
        << " -> " 
        << static_cast<char>('a' + to.col) << (8 - to.row);
    
    if (isCastling) oss << " (Castling)";
    if (isPromotion) oss << " (Promotion)";
    
    return oss.str();
}