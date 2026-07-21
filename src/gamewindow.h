#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include "gamecontroller.h"

class GameWindow : public QObject {
    Q_OBJECT
    Q_PROPERTY(Color currentPlayer READ getCurrentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(GameState gameState READ getGameState NOTIFY gameStateChanged)
    Q_PROPERTY(QVariantList moveHistory READ getMoveHistory NOTIFY historyChanged)
    Q_PROPERTY(bool isNetworkGame READ isNetworkGame NOTIFY networkGameChanged)
    Q_PROPERTY(bool isServer READ isServer NOTIFY networkGameChanged)
    Q_PROPERTY(bool canMakeMove READ canMakeMove NOTIFY canMakeMoveChanged)
    Q_PROPERTY(Color myColor READ getMyColor NOTIFY myColorChanged)
    Q_PROPERTY(bool isConnecting READ isConnecting NOTIFY isConnectingChanged)

public:
    explicit GameWindow(QObject *parent = nullptr);

    Q_INVOKABLE void startNewGame(int mode, int playerColor);
    Q_INVOKABLE void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE QVariantList getBoardState() const;
    Q_INVOKABLE QVariantList getMoveHistory() const;
    Q_INVOKABLE void saveGame(const QString& filename);
    Q_INVOKABLE void selectPiece(int row, int col);
    Q_INVOKABLE void clearSelection();
    
    // Сетевые методы, доступные из QML
    Q_INVOKABLE void startServer(int port);
    Q_INVOKABLE void connectToServer(const QString& ip, int port);

    Color getCurrentPlayer() const;
    GameState getGameState() const;
    bool isNetworkGame() const { return controller.mode == GameMode::Network; }
    bool isServer() const { return controller.networkManager && controller.networkManager->server()->isListening(); }
    bool canMakeMove() const;
    Color getMyColor() const { return myColor; }
    bool isConnecting() const { return isAttemptingConnection; }

signals:
    void currentPlayerChanged();
    void gameStateChanged();
    void messageShown(const QString& message);
    void boardUpdated();
    void historyChanged();
    void networkGameChanged();
    void connectionStatusChanged(const QString& status);
    void canMakeMoveChanged();
    void myColorChanged();
    void isConnectingChanged();
    void connectionError(const QString& error);

private slots:
    void onNetworkMoveReceived(int fromRow, int fromCol, int toRow, int toCol);
    void onConnectionEstablished();
    void onConnectionError(const QString& error);

private:
    GameController controller;
    Position selectedPosition;
    bool isPieceSelected;
    bool isConnectionReady;
    Color myColor;
    bool isAttemptingConnection;
};

#endif // GAMEWINDOW_H