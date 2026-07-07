#pragma once

#include <QObject>

#include "networkserver.h"
#include "networkclient.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);

    bool createServer(quint16 port);
    void connectToHost(const QString& ip, quint16 port);

    NetworkServer* server();
    NetworkClient* client();

private:
    NetworkServer m_server;
    NetworkClient m_client;
};