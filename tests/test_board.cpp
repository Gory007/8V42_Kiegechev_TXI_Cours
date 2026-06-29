#include <gtest/gtest.h>
#include "board.h"
#include <vector>
#include <iostream>

// Заглушка для qDebug, если она нужна
#ifdef QT_CORE_LIB
    #include <QDebug>
#else
    #define qDebug() std::cout
#endif

class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        board.generateInitialPosition();
    }
    
    Board board;
};

// Тест 1: Симметричность расстановки (белые vs черные)
TEST_F(BoardTest, SymmetryBetweenColors) {
    for (int col = 0; col < 8; ++col) {
        Piece* whitePiece = board.getPiece(Position(7, col));
        Piece* blackPiece = board.getPiece(Position(0, col));
        
        ASSERT_NE(whitePiece, nullptr) << "No white piece at column " << col;
        ASSERT_NE(blackPiece, nullptr) << "No black piece at column " << col;
        
        // Типы должны совпадать
        EXPECT_EQ(whitePiece->getType(), blackPiece->getType()) 
            << "Piece type mismatch at column " << col;
    }
}

// Тест 2: Корректное количество каждого типа фигур
TEST_F(BoardTest, CorrectPieceCounts) {
    int whiteCounts[6] = {0};
    int blackCounts[6] = {0};
    
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* piece = board.getPiece(Position(r, c));
            if (piece) {
                int typeIdx = static_cast<int>(piece->getType());
                if (piece->color == Color::White) {
                    whiteCounts[typeIdx]++;
                } else {
                    blackCounts[typeIdx]++;
                }
            }
        }
    }
    
    EXPECT_EQ(whiteCounts[0], 1) << "White king count incorrect";
    EXPECT_EQ(blackCounts[0], 1) << "Black king count incorrect";
    EXPECT_EQ(whiteCounts[1], 1) << "White queen count incorrect";
    EXPECT_EQ(blackCounts[1], 1) << "Black queen count incorrect";
    EXPECT_EQ(whiteCounts[2], 2) << "White rook count incorrect";
    EXPECT_EQ(blackCounts[2], 2) << "Black rook count incorrect";
    EXPECT_EQ(whiteCounts[3], 2) << "White bishop count incorrect";
    EXPECT_EQ(blackCounts[3], 2) << "Black bishop count incorrect";
    EXPECT_EQ(whiteCounts[4], 2) << "White knight count incorrect";
    EXPECT_EQ(blackCounts[4], 2) << "Black knight count incorrect";
    EXPECT_EQ(whiteCounts[5], 8) << "White pawn count incorrect";
    EXPECT_EQ(blackCounts[5], 8) << "Black pawn count incorrect";
}

// Тест 3: Пешки на правильных рядах
TEST_F(BoardTest, PawnsOnCorrectRows) {
    for (int col = 0; col < 8; ++col) {
        Piece* whitePawn = board.getPiece(Position(6, col));
        Piece* blackPawn = board.getPiece(Position(1, col));
        
        ASSERT_NE(whitePawn, nullptr) << "Missing white pawn at (6," << col << ")";
        ASSERT_NE(blackPawn, nullptr) << "Missing black pawn at (1," << col << ")";
        
        EXPECT_EQ(whitePawn->getType(), PieceType::Pawn);
        EXPECT_EQ(blackPawn->getType(), PieceType::Pawn);
        EXPECT_EQ(whitePawn->color, Color::White);
        EXPECT_EQ(blackPawn->color, Color::Black);
    }
}

// Тест 4: Слоны на разных цветах полей (Chess960 правило)
TEST_F(BoardTest, BishopsOnDifferentColors) {
    std::vector<int> whiteBishopCols;
    std::vector<int> blackBishopCols;
    
    for (int col = 0; col < 8; ++col) {
        Piece* whitePiece = board.getPiece(Position(7, col));
        Piece* blackPiece = board.getPiece(Position(0, col));
        
        if (whitePiece && whitePiece->getType() == PieceType::Bishop) {
            whiteBishopCols.push_back(col);
        }
        if (blackPiece && blackPiece->getType() == PieceType::Bishop) {
            blackBishopCols.push_back(col);
        }
    }
    
    ASSERT_EQ(whiteBishopCols.size(), 2) << "White should have 2 bishops";
    ASSERT_EQ(blackBishopCols.size(), 2) << "Black should have 2 bishops";
    
    bool whiteBishopsOnDifferentColors = 
        ((7 + whiteBishopCols[0]) % 2) != ((7 + whiteBishopCols[1]) % 2);
    bool blackBishopsOnDifferentColors = 
        ((0 + blackBishopCols[0]) % 2) != ((0 + blackBishopCols[1]) % 2);
    
    EXPECT_TRUE(whiteBishopsOnDifferentColors);
    EXPECT_TRUE(blackBishopsOnDifferentColors);
}

// Тест 5: Король между ладьями
TEST_F(BoardTest, KingBetweenRooks) {
    int whiteKingPos = -1;
    std::vector<int> whiteRookPos;
    int blackKingPos = -1;
    std::vector<int> blackRookPos;
    
    for (int col = 0; col < 8; ++col) {
        Piece* whitePiece = board.getPiece(Position(7, col));
        Piece* blackPiece = board.getPiece(Position(0, col));
        
        if (whitePiece && whitePiece->getType() == PieceType::King) {
            whiteKingPos = col;
        }
        if (whitePiece && whitePiece->getType() == PieceType::Rook) {
            whiteRookPos.push_back(col);
        }
        if (blackPiece && blackPiece->getType() == PieceType::King) {
            blackKingPos = col;
        }
        if (blackPiece && blackPiece->getType() == PieceType::Rook) {
            blackRookPos.push_back(col);
        }
    }
    
    ASSERT_NE(whiteKingPos, -1);
    ASSERT_EQ(whiteRookPos.size(), 2);
    ASSERT_NE(blackKingPos, -1);
    ASSERT_EQ(blackRookPos.size(), 2);
    
    bool whiteKingBetweenRooks = (whiteRookPos[0] < whiteKingPos && whiteKingPos < whiteRookPos[1]) ||
                                  (whiteRookPos[1] < whiteKingPos && whiteKingPos < whiteRookPos[0]);
    bool blackKingBetweenRooks = (blackRookPos[0] < blackKingPos && blackKingPos < blackRookPos[1]) ||
                                  (blackRookPos[1] < blackKingPos && blackKingPos < blackRookPos[0]);
    
    EXPECT_TRUE(whiteKingBetweenRooks);
    EXPECT_TRUE(blackKingBetweenRooks);
}

// Тест 6: Проверка симметрии при многократной генерации
TEST_F(BoardTest, MultipleGenerationsAreSymmetric) {
    const int testRuns = 50; // Уменьшил для быстроты
    
    for (int run = 0; run < testRuns; ++run) {
        board.generateInitialPosition();
        
        for (int col = 0; col < 8; ++col) {
            Piece* whitePiece = board.getPiece(Position(7, col));
            Piece* blackPiece = board.getPiece(Position(0, col));
            
            ASSERT_NE(whitePiece, nullptr);
            ASSERT_NE(blackPiece, nullptr);
            
            EXPECT_EQ(whitePiece->getType(), blackPiece->getType())
                << "Run " << run << ", column " << col;
        }
    }
}

// Тест 7: Нет фигур на пустых рядах
TEST_F(BoardTest, EmptyRowsAreEmpty) {
    for (int r = 2; r <= 5; ++r) {
        for (int c = 0; c < 8; ++c) {
            EXPECT_EQ(board.getPiece(Position(r, c)), nullptr) 
                << "Piece found at (" << r << "," << c << ")";
        }
    }
}