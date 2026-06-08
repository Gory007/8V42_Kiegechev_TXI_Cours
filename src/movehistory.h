#ifndef MOVEHISTORY_H
#define MOVEHISTORY_H

#include "move.h"
#include <vector>

class MoveHistory {
public:
    std::vector<Move> moves;

    void addMove(const Move& move);
    std::vector<Move> getAll() const;
    void clear();
};

#endif // MOVEHISTORY_H