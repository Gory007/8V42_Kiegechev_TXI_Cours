#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_server, &NetworkServer::moveReceived, this, &NetworkManager::moveReceived);
    connect(&m_server, &NetworkServer::clientConnected, this, &NetworkManager::clientConnected);
    
    connect(&m_client, &NetworkClient::moveReceived, this, &NetworkManager::moveReceived);
    connect(&m_client, &NetworkClient::connectedToServer, this, &NetworkManager::connectedToServer);
}

bool NetworkManager::createServer(quint16 port)
{
    m_isServer = true;
    return m_server.startServer(port);
}

void NetworkManager::connectToHost(const QString &ip, quint16 port)
{
    m_isServer = false;
    m_client.connectToServer(ip, port);
}

void NetworkManager::sendMove(const Move &move)
{
    if (m_isServer) {
        m_server.sendMove(move);
    } else {
        m_client.sendMove(move);
    }
}

NetworkServer* NetworkManager::server()
{
    return &m_server;
}

NetworkClient* NetworkManager::client()
{
    return &m_client;
}