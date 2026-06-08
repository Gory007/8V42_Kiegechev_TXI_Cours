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

public:
    explicit GameWindow(QObject *parent = nullptr);

    Q_INVOKABLE void startNewGame(int mode, int playerColor);
    Q_INVOKABLE void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE QVariantList getBoardState() const;
    Q_INVOKABLE QVariantList getMoveHistory() const;
    Q_INVOKABLE void saveGame(const QString& filename);
    Q_INVOKABLE void selectPiece(int row, int col);
    Q_INVOKABLE void clearSelection();
    
    Color getCurrentPlayer() const;
    GameState getGameState() const;

signals:
    void currentPlayerChanged();
    void gameStateChanged();
    void messageShown(const QString& message);
    void boardUpdated();
    void historyChanged();

private:
    GameController controller;
    Position selectedPosition;
    bool isPieceSelected;
};

#endif // GAMEWINDOW_H