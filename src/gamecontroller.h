#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include "board.h"
#include "movehistory.h"
#include "aiplayer.h"
#include "network/networkmanager.h"

enum class GameMode {
    PvP,
    PvAI,
    Network
};

enum class GameState {
    Playing,
    Check,
    Checkmate,
    Stalemate
};

class GameController : public QObject {
    Q_OBJECT

public:
    Board board;
    MoveHistory history;
    AIPlayer* aiPlayer;
    Color currentPlayer;
    GameMode mode;
    NetworkManager* networkManager;

    GameController();
    ~GameController();

    void startNewGame(GameMode mode, Color playerColor);
    bool handleMove(Position from, Position to);
    bool validateMove(Position from, Position to) const;
    GameState checkGameState() const;
    void saveHistoryToFile(const std::string& path);

    // Сетевые методы
    void startServer(quint16 port);
    void connectToServer(const QString& ip, quint16 port);
    void handleNetworkMove(const Move& move);

    // Геттеры для UI
    Color getCurrentPlayer() const { return currentPlayer; }
    const Board& getBoard() const { return board; }

signals:
    void networkMoveReceived(int fromRow, int fromCol, int toRow, int toCol);
    void connectionEstablished();

private slots:
    void onNetworkMoveReceived(const Move& move);
};

#endif // GAMECONTROLLER_H