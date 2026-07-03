#pragma once

#include <QTcpSocket>

class NetworkClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);
};