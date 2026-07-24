#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <vector>
#include "networkserver.h"
#include "networkclient.h"
#include "move.h"

class NetworkManager : public QObject {
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    void createServer(quint16 port);
    void connectToHost(const QString& ip, quint16 port);
    void sendMove(const Move& move);
    void setPendingInitPosition(const std::vector<int>& backRank);
    void stopNetwork(); // НОВЫЙ МЕТОД: остановить сеть

    NetworkServer* server() const { return m_server; }
    NetworkClient* client() const { return m_client; }

signals:
    void moveReceived(const Move& move);
    void clientConnected();
    void connectedToServer();
    void initPositionReceived(const std::vector<int>& backRank);
    void networkDisconnected(); // НОВЫЙ СИГНАЛ

private slots:
    void onMoveReceived(const Move& move);
    void onClientConnected();
    void onConnectedToServer();
    void onInitPositionReceived(const std::vector<int>& backRank);
    void onNetworkDisconnected(); // НОВЫЙ СЛОТ

private:
    NetworkServer* m_server;
    NetworkClient* m_client;
    std::vector<int> m_pendingInitPosition;
};

#endif // NETWORKMANAGER_H