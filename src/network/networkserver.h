#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
#include "move.h"

class NetworkServer : public QObject {
    Q_OBJECT

public:
    NetworkServer(QObject *parent = nullptr);
    ~NetworkServer();

    void startServer(quint16 port);
    void sendMove(const Move& move);
    void sendInitPosition(const std::vector<int>& backRank);
    
    void setPendingInitPosition(const std::vector<int>& backRank);
    
    QTcpServer* server() const { return m_server; }
    bool isListening() const { return m_server && m_server->isListening(); }

signals:
    void moveReceived(const Move& move);
    void clientConnected();
    void initPositionSent();

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    void readMove(QTcpSocket* socket);
    void sendInitPositionToClient(QTcpSocket* client);

    QTcpServer* m_server;
    QTcpSocket* m_clientSocket;
    std::vector<int> m_pendingInitPosition;
};

#endif // NETWORKSERVER_H