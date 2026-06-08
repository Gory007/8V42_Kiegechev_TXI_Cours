#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "board.h"
#include "move.h"

class AIPlayer {
public:
    Color color;
    AIPlayer(Color color) : color(color) {}
    Move makeMove(const Board& board);
    
private:
    int evaluateMove(const Board& board, const Move& move) const;
    int getPieceValue(PieceType type) const;
};

#endif // AIPLAYER_H