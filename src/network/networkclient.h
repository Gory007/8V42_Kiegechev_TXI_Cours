#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <vector>
#include "move.h"

class NetworkClient : public QObject {
    Q_OBJECT

public:
    NetworkClient(QObject *parent = nullptr);
    ~NetworkClient();

    void connectToServer(const QString& ip, quint16 port);
    void sendMove(const Move& move);
    
    QTcpSocket* socket() const { return m_socket; }
    bool isConnected() const { return m_socket && m_socket->state() == QAbstractSocket::ConnectedState; }

signals:
    void moveReceived(const Move& move);
    void connected();
    void connectionError(const QString& error);
    void initPositionReceived(const std::vector<int>& backRank); // НОВЫЙ СИГНАЛ

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReadyRead();

private:
    void readMessage();

    QTcpSocket* m_socket;
};

#endif // NETWORKCLIENT_H