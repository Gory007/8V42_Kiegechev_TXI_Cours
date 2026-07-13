#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "board.h"
#include "movehistory.h"
#include "aiplayer.h"
#include <string>

enum class GameMode {
    PvP,
    PvAI
};

enum class GameState {
    Playing,
    Check,
    Checkmate,
    Stalemate
};

class GameController {
public:
    Board board;
    MoveHistory history;
    AIPlayer* aiPlayer;
    Color currentPlayer;
    GameMode mode;

    GameController();
    ~GameController();

    void startNewGame(GameMode mode, Color playerColor);
    bool handleMove(Position from, Position to);
    bool validateMove(Position from, Position to) const;
    GameState checkGameState() const;
    void saveHistoryToFile(const std::string& path);
    
    // Геттеры для UI
    Color getCurrentPlayer() const { return currentPlayer; }
    const Board& getBoard() const { return board; }
};

#endif // GAMECONTROLLER_H