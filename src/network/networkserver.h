#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "../move.h"

class NetworkServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = nullptr);

    bool startServer(quint16 port);
    void stopServer();
    void sendMove(const Move& move);
    QTcpSocket* clientSocket() const;

signals:
    void clientConnected();
    void moveReceived(const Move& move);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onClientReadyRead();

private:
    QTcpSocket* m_clientSocket = nullptr;
    Move parseMoveFromJson(const QJsonObject& json);
};