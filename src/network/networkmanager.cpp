#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_server(new NetworkServer(this))
    , m_client(new NetworkClient(this))
{
    connect(m_server, &NetworkServer::moveReceived, this, &NetworkManager::onMoveReceived);
    connect(m_server, &NetworkServer::clientConnected, this, &NetworkManager::onClientConnected);
    connect(m_server, &NetworkServer::clientDisconnected, this, &NetworkManager::onNetworkDisconnected); // НОВОЕ
    connect(m_server, &NetworkServer::initPositionSent, this, [this]() {
        qDebug() << "Initial position sent to client";
    });

    connect(m_client, &NetworkClient::moveReceived, this, &NetworkManager::onMoveReceived);
    connect(m_client, &NetworkClient::connected, this, &NetworkManager::onConnectedToServer);
    connect(m_client, &NetworkClient::disconnected, this, &NetworkManager::onNetworkDisconnected); // НОВОЕ
    connect(m_client, &NetworkClient::initPositionReceived, this, &NetworkManager::onInitPositionReceived);
}

NetworkManager::~NetworkManager() {
    stopNetwork();
}

void NetworkManager::createServer(quint16 port) {
    m_server->startServer(port);
}

void NetworkManager::connectToHost(const QString& ip, quint16 port) {
    m_client->connectToServer(ip, port);
}

void NetworkManager::sendMove(const Move& move) {
    if (m_server && m_server->isListening()) {
        m_server->sendMove(move);
    } else if (m_client && m_client->isConnected()) {
        m_client->sendMove(move);
    }
}

void NetworkManager::setPendingInitPosition(const std::vector<int>& backRank) {
    m_pendingInitPosition = backRank;
    if (m_server) {
        m_server->setPendingInitPosition(backRank);
    }
}

void NetworkManager::stopNetwork() {
    m_server->stopServer();
    m_client->disconnectFromServer();
    m_pendingInitPosition.clear();
}

void NetworkManager::onMoveReceived(const Move& move) {
    emit moveReceived(move);
}

void NetworkManager::onClientConnected() {
    emit clientConnected();
}

void NetworkManager::onConnectedToServer() {
    emit connectedToServer();
}

void NetworkManager::onInitPositionReceived(const std::vector<int>& backRank) {
    emit initPositionReceived(backRank);
}

void NetworkManager::onNetworkDisconnected() {
    qDebug() << "Network disconnected";
    emit networkDisconnected(); // НОВЫЙ СИГНАЛ
}