#pragma once

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "../move.h"

class NetworkClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString& ip, quint16 port);
    void disconnectFromServer();
    void sendMove(const Move& move);

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void moveReceived(const Move& move);
    void connectionError(const QString& error); 

private slots:
    void onReadyRead();

private:
    Move parseMoveFromJson(const QJsonObject& json);
};