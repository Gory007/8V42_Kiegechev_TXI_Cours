#pragma once

#include <QTcpSocket>

class NetworkClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString& ip, quint16 port);
    void disconnectFromServer();

signals:
    void connectedToServer();
    void disconnectedFromServer();
};