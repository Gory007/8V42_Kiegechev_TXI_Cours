#ifndef POSITION_H
#define POSITION_H

struct Position {
    int row;
    int col;

    // Конструктор по умолчанию
    Position() : row(0), col(0) {}
    Position(int r, int c) : row(r), col(c) {}

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

#endif // POSITION_H