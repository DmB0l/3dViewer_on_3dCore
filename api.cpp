#include "api.h"

API::API(QObject *parent)
    : QObject{parent},
    m_listenSocket(new QUdpSocket)
{
    m_listenSocket->bind(QHostAddress(m_listenIp), m_listenPort);
    connect(m_listenSocket, &QUdpSocket::readyRead, this, &API::readPendingDatagrams);
}

void API::readPendingDatagrams()
{
    // QByteArray buffer;
    // while (m_listenSocket->hasPendingDatagrams()) {
    //     QByteArray datagram;
    //     datagram.resize(m_listenSocket->pendingDatagramSize());

    //     qint64 bytesRead = m_listenSocket->readDatagram(datagram.data(), datagram.size());
    //     if (bytesRead != -1) {
    //         buffer.append(datagram);
    //     } else {
    //         qDebug() << "Ошибка чтения датаграммы:" << m_listenSocket->errorString();
    //     }
    // }

    QByteArray buffer;
    buffer.resize(m_listenSocket->pendingDatagramSize());
    qint64 bytesRead = m_listenSocket->readDatagram(buffer.data(), buffer.size());
    if (bytesRead < 0) {
        qDebug() << "Ошибка чтения датаграммы:" << m_listenSocket->errorString();
    }

    // Парсинг JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(buffer);
    if (jsonDoc.isNull()) {
        qDebug() << "Ошибка парсинга JSON:" << buffer;
        return;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "Ожидался объект JSON, но получен другой тип.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    // Извлечение данных
    QString type = jsonObj.value("type").toString();
    if (type == "line") {
        QJsonArray pointsArray = jsonObj.value("points").toArray();
        QString color = jsonObj.value("color").toString();

        // Проверка на наличие необходимых данных
        if (pointsArray.size() < 6) {
            qDebug() << "Недостаточно точек для линии.";
            return;
        }

        // Извлечение координат
        qreal x1 = pointsArray[0].toDouble();
        qreal y1 = pointsArray[1].toDouble();
        qreal z1 = pointsArray[2].toDouble();
        qreal x2 = pointsArray[3].toDouble();
        qreal y2 = pointsArray[4].toDouble();
        qreal z2 = pointsArray[5].toDouble();

        // Здесь вы можете обработать полученные данные
        qDebug() << "Получена полилиния с точками:" << x1 << y1 << z1 << x2 << y2 << z2 << "и цветом:" << color;

        emit addLine(x1, y1, z1, x2, y2, z2, QColor(color));

        // Вызов функции для обработки полилинии
        // processPolyline(x1, y1, x2, y2, color);
    }
    else if(type == "cube") {
        QJsonArray centerArray = jsonObj.value("center").toArray();
        QString color = jsonObj.value("color").toString();
        int size = jsonObj.value("size").toInt();

        // Проверка на наличие необходимых данных
        if (centerArray.size() < 3) {
            qDebug() << "Недостаточно точек для центра куба.";
            return;
        }

        // Извлечение координат
        qreal x = centerArray[0].toDouble();
        qreal y = centerArray[1].toDouble();
        qreal z = centerArray[2].toDouble();

        // Здесь вы можете обработать полученные данные
        qDebug() << "Получен cube с центром:" << x << y << z << "размером:" << size << "и цветом:" << color;

        emit addCube(x, y, z, size, QColor(color));
    }
    else {
        qDebug() << "Неизвестный тип сообщения:" << type;
    }
}
