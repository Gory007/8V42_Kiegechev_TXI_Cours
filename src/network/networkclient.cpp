#include "networkclient.h"

NetworkClient::NetworkClient(QObject *parent)
    : QTcpSocket(parent)
{
}