#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent)
    : QTcpServer(parent)
{
}