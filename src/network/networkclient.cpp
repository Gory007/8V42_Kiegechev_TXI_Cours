#include "networkclient.h"
#include <QDebug>
#include <QDataStream>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkClient::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &NetworkClient::onError);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
}

NetworkClient::~NetworkClient() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

void NetworkClient::connectToServer(const QString& ip, quint16 port) {
    qDebug() << "Connecting to" << ip << ":" << port;
    m_socket->connectToHost(ip, port);
}

void NetworkClient::disconnectFromServer() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Disconnecting from server";
        m_socket->disconnectFromHost();
    }
}

void NetworkClient::onConnected() {
    qDebug() << "Connected to server";
    emit connected();
}

void NetworkClient::onDisconnected() {
    qDebug() << "Disconnected from server";
    emit disconnected(); // НОВЫЙ СИГНАЛ
}

void NetworkClient::onError(QAbstractSocket::SocketError error) {
    QString errorMsg = m_socket->errorString();
    qDebug() << "Socket error:" << errorMsg;
    emit connectionError(errorMsg);
}

void NetworkClient::onReadyRead() {
    readMessage();
}

void NetworkClient::readMessage() {
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_12);

    if (m_socket->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }

    quint16 msgSize;
    in >> msgSize;

    if (m_socket->bytesAvailable() < msgSize) {
        return;
    }

    QString messageType;
    in >> messageType;

    if (messageType == "INIT") {
        qint32 size;
        in >> size;
        
        std::vector<int> backRank;
        for (qint32 i = 0; i < size; ++i) {
            qint32 piece;
            in >> piece;
            backRank.push_back(static_cast<int>(piece));
        }
        
        qDebug() << "Received initial position from server, size:" << backRank.size();
        emit initPositionReceived(backRank);
        return;
    }

    if (messageType == "MOVE") {
        int fromRow, fromCol, toRow, toCol;
        int pieceType, capturedType;
        bool isCastling, isPromotion;

        in >> fromRow >> fromCol >> toRow >> toCol;
        in >> pieceType >> capturedType >> isCastling >> isPromotion;

        Move move(Position(fromRow, fromCol), Position(toRow, toCol),
                  static_cast<PieceType>(pieceType), static_cast<PieceType>(capturedType),
                  isCastling, isPromotion);

        emit moveReceived(move);
        return;
    }

    qDebug() << "Unknown message type:" << messageType;
}

void NetworkClient::sendMove(const Move& move) {
    if (m_socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Cannot send move: not connected";
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

    m_socket->write(block);
    qDebug() << "Move sent to server:" << move.from.row << move.from.col << "->" << move.to.row << move.to.col;
}