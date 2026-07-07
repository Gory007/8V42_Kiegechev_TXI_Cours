#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent)
    : QTcpServer(parent)
{
}

bool NetworkServer::startServer(quint16 port)
{
    return listen(QHostAddress::Any, port);
}

void NetworkServer::stopServer()
{
    close();

    if (m_clientSocket)
    {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    m_clientSocket = new QTcpSocket(this);

    if (m_clientSocket->setSocketDescriptor(socketDescriptor))
    {
        emit clientConnected();
    }
    else
    {
        delete m_clientSocket;
        m_clientSocket = nullptr;
    }
}

QTcpSocket *NetworkServer::clientSocket() const
{
    return m_clientSocket;
}