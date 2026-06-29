#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "position.h"

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

    void generateInitialPosition(); // Генерация случайной расстановки Chess960
    void clearBoard();

    std::vector<int> generateRandomChess960Position();

private:
    // Вспомогательная функция для проверки, что слоны на разных цветах
    bool areBishopsOnDifferentColors(const std::vector<int>& position) const;
    // Вспомогательная функция для проверки, что король между ладьями
    bool isKingBetweenRooks(const std::vector<int>& position) const;
};

#endif // BOARD_H