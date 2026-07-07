#pragma once

#include <QTcpServer>
#include <QTcpSocket>

class NetworkServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = nullptr);

    bool startServer(quint16 port);
    void stopServer();

    QTcpSocket* clientSocket() const;

signals:
    void clientConnected();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QTcpSocket* m_clientSocket = nullptr;
};