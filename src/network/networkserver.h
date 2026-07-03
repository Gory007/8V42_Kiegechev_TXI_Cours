#pragma once

#include <QTcpServer>

class NetworkServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = nullptr);
};