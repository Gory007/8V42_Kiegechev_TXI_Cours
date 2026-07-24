#include "networkserver.h"
#include <QDebug>
#include <QDataStream>

NetworkServer::NetworkServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
    , m_clientSocket(nullptr)
{
    connect(m_server, &QTcpServer::newConnection, this, &NetworkServer::onNewConnection);
}

NetworkServer::~NetworkServer() {
    stopServer();
}

void NetworkServer::startServer(quint16 port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start:" << m_server->errorString();
        return;
    }
    qDebug() << "Server started on port" << port;
}

void NetworkServer::stopServer() {
    if (m_clientSocket) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket = nullptr;
    }
    if (m_server->isListening()) {
        m_server->close();
        qDebug() << "Server stopped";
    }
}

void NetworkServer::onNewConnection() {
    m_clientSocket = m_server->nextPendingConnection();
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetworkServer::onClientDisconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetworkServer::onReadyRead);
    
    qDebug() << "Client connected";
    emit clientConnected();
    
    sendInitPositionToClient(m_clientSocket);
}

void NetworkServer::onClientDisconnected() {
    qDebug() << "Client disconnected";
    m_clientSocket = nullptr;
    emit clientDisconnected(); // НОВЫЙ СИГНАЛ
}

void NetworkServer::onReadyRead() {
    readMove(m_clientSocket);
}

void NetworkServer::readMove(QTcpSocket* socket) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);

    if (socket->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }

    quint16 msgSize;
    in >> msgSize;

    if (socket->bytesAvailable() < msgSize) {
        return;
    }

    QString messageType;
    in >> messageType;

    if (messageType != "MOVE") {
        qDebug() << "Unknown message type:" << messageType;
        return;
    }

    int fromRow, fromCol, toRow, toCol;
    int pieceType, capturedType;
    bool isCastling, isPromotion;

    in >> fromRow >> fromCol >> toRow >> toCol;
    in >> pieceType >> capturedType >> isCastling >> isPromotion;

    Move move(Position(fromRow, fromCol), Position(toRow, toCol),
              static_cast<PieceType>(pieceType), static_cast<PieceType>(capturedType),
              isCastling, isPromotion);

    emit moveReceived(move);
}

void NetworkServer::sendMove(const Move& move) {
    if (!m_clientSocket || m_clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Cannot send move: client not connected";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    out << quint16(0);
    out << QString("MOVE");
    out << move.from.row << move.from.col << move.to.row << move.to.col;
    out << static_cast<int>(move.piece) << static_cast<int>(move.captured);
    out << move.isCastling << move.isPromotion;

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    m_clientSocket->write(block);
    qDebug() << "Move sent to client:" << move.from.row << move.from.col << "->" << move.to.row << move.to.col;
}

void NetworkServer::setPendingInitPosition(const std::vector<int>& backRank) {
    m_pendingInitPosition = backRank;
}

void NetworkServer::sendInitPosition(const std::vector<int>& backRank) {
    if (!m_clientSocket || m_clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Cannot send init position: client not connected";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    out << quint16(0);
    out << QString("INIT");
    out << static_cast<qint32>(backRank.size());
    for (int piece : backRank) {
        out << static_cast<qint32>(piece);
    }

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    m_clientSocket->write(block);
    qDebug() << "Initial position sent to client";
    emit initPositionSent();
}

void NetworkServer::sendInitPositionToClient(QTcpSocket* client) {
    if (m_pendingInitPosition.empty()) {
        qDebug() << "No pending init position to send";
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    out << quint16(0);
    out << QString("INIT");
    out << static_cast<qint32>(m_pendingInitPosition.size());
    for (int piece : m_pendingInitPosition) {
        out << static_cast<qint32>(piece);
    }

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    client->write(block);
    qDebug() << "Initial position sent to newly connected client";
    emit initPositionSent();
}