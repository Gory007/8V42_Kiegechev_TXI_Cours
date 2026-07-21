#include "networkclient.h"

NetworkClient::NetworkClient(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::connected, this, &NetworkClient::connectedToServer);
    connect(this, &QTcpSocket::disconnected, this, &NetworkClient::disconnectedFromServer);
    connect(this, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);

    // Обработка ошибок подключения
    connect(this, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError);
        QString errorMsg = this->errorString();
        qWarning() << "NetworkClient error:" << errorMsg;
        emit connectionError(errorMsg);
    });
}

void NetworkClient::connectToServer(const QString &ip, quint16 port)
{
    connectToHost(ip, port);
}

void NetworkClient::disconnectFromServer()
{
    disconnectFromHost();
}

void NetworkClient::sendMove(const Move &move)
{
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
    write(doc.toJson(QJsonDocument::Compact));
    write("\n");
    flush();
}

void NetworkClient::onReadyRead()
{
    QByteArray data = readAll();
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

Move NetworkClient::parseMoveFromJson(const QJsonObject &json)
{
    Position from(json["from_row"].toInt(), json["from_col"].toInt());
    Position to(json["to_row"].toInt(), json["to_col"].toInt());
    PieceType piece = static_cast<PieceType>(json["piece"].toInt());
    PieceType captured = static_cast<PieceType>(json["captured"].toInt());
    bool isCastling = json["is_castling"].toBool();
    bool isPromotion = json["is_promotion"].toBool();

    return Move(from, to, piece, captured, isCastling, isPromotion);
}