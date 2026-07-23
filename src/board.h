#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "position.h"
#include <vector>

class Board {
public:
    Piece* cells[8][8]; // Указатели на фигуры, как в UML

    Board();
    ~Board();

    Piece* getPiece(Position pos) const;
    void movePiece(Position from, Position to);
    bool isCellEmpty(Position pos) const;
    Board clone() const; // Глубокое копирование доски

    bool isSquareAttacked(Position pos, Color byColor) const;
    Position findKing(Color color) const;

    // ИЗМЕНЕНО: теперь возвращает backRank для отправки по сети
    std::vector<int> generateInitialPosition(); 
    // НОВЫЙ МЕТОД: для установки позиции, полученной от сервера
    void setInitialPosition(const std::vector<int>& backRank); 
    
    void clearBoard();
    std::vector<int> generateRandomChess960Position();

private:
    bool areBishopsOnDifferentColors(const std::vector<int>& position) const;
    bool isKingBetweenRooks(const std::vector<int>& position) const;
};

#endif // BOARD_H