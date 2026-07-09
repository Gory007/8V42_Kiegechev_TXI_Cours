#include "networkserver.h"

NetworkServer::NetworkServer(QObject *parent)
    : QTcpServer(parent)
{
}

bool NetworkServer::startServer(quint16 port)
{
    return listen(QHostAddress::Any, port);
}

void NetworkServer::stopServer()
{
    close();

    if (m_clientSocket)
    {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    m_clientSocket = new QTcpSocket(this);

    if (m_clientSocket->setSocketDescriptor(socketDescriptor))
    {
        connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetworkServer::onClientReadyRead);
        emit clientConnected();
    }
    else
    {
        delete m_clientSocket;
        m_clientSocket = nullptr;
    }
}

void NetworkServer::sendMove(const Move &move)
{
    if (!m_clientSocket) return;

    QJsonObject json;
    json["type"] = "move";
    json["from_row"] = move.from.row;
    json["from_col"] = move.from.col;
    json["to_row"] = move.to.row;
    json["to_col"] = move.to.col;
    json["piece"] = static_cast<int>(move.piece);
    json["captured"] = static_cast<int>(move.captured);
    json["is_castling"] = move.isCastling;
    json["is_promotion"] = move.isPromotion;

    QJsonDocument doc(json);
    m_clientSocket->write(doc.toJson(QJsonDocument::Compact));
    m_clientSocket->write("\n");
    m_clientSocket->flush();
}

void NetworkServer::onClientReadyRead()
{
    QByteArray data = m_clientSocket->readAll();
    QString strData = QString::fromUtf8(data);
    
    QStringList lines = strData.split('\n', Qt::SkipEmptyParts);
    
    for (const QString& line : lines) {
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
        if (doc.isNull()) continue;
        
        QJsonObject json = doc.object();
        if (json["type"] == "move") {
            Move move = parseMoveFromJson(json);
            emit moveReceived(move);
        }
    }
}

Move NetworkServer::parseMoveFromJson(const QJsonObject &json)
{
    Position from(json["from_row"].toInt(), json["from_col"].toInt());
    Position to(json["to_row"].toInt(), json["to_col"].toInt());
    PieceType piece = static_cast<PieceType>(json["piece"].toInt());
    PieceType captured = static_cast<PieceType>(json["captured"].toInt());
    bool isCastling = json["is_castling"].toBool();
    bool isPromotion = json["is_promotion"].toBool();

    return Move(from, to, piece, captured, isCastling, isPromotion);
}

QTcpSocket *NetworkServer::clientSocket() const
{
    return m_clientSocket;
}