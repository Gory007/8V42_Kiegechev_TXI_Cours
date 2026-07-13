#pragma once

#include <QObject>
#include "networkserver.h"
#include "networkclient.h"
#include "../move.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);

    bool createServer(quint16 port);
    void connectToHost(const QString& ip, quint16 port);
    void sendMove(const Move& move);

    NetworkServer* server();
    NetworkClient* client();

signals:
    void moveReceived(const Move& move);
    void clientConnected();
    void connectedToServer();

private:
    NetworkServer m_server;
    NetworkClient m_client;
    bool m_isServer = false;
};