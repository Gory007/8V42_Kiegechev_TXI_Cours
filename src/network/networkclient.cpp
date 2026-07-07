#include "networkclient.h"

NetworkClient::NetworkClient(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this,
            &QTcpSocket::connected,
            this,
            &NetworkClient::connectedToServer);

    connect(this,
            &QTcpSocket::disconnected,
            this,
            &NetworkClient::disconnectedFromServer);
}

void NetworkClient::connectToServer(const QString &ip, quint16 port)
{
    connectToHost(ip, port);
}

void NetworkClient::disconnectFromServer()
{
    disconnectFromHost();
}