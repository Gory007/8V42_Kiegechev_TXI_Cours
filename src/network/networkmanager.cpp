#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
}

bool NetworkManager::createServer(quint16 port)
{
    return m_server.startServer(port);
}

void NetworkManager::connectToHost(const QString &ip, quint16 port)
{
    m_client.connectToServer(ip, port);
}

NetworkServer *NetworkManager::server()
{
    return &m_server;
}

NetworkClient *NetworkManager::client()
{
    return &m_client;
}