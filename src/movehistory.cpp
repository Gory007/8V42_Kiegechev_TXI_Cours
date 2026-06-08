#include "movehistory.h"

void MoveHistory::addMove(const Move& move) {
    moves.push_back(move);
}

std::vector<Move> MoveHistory::getAll() const {
    return moves;
}

void MoveHistory::clear() {
    moves.clear();
}