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
    if(jsonObj.size() >= 1) {

        // LINE
        QString type = jsonObj.value("type").toString();
        if (type == "line") {
            if(jsonObj.size() != 3) {
                qDebug() << "Недостаточно параметров для линии.";
                return;
            }
            if(!jsonObj.contains("points") || !jsonObj.contains("color")) {
                qDebug() << "Не те параметры для линии.";
                return;
            }

            QJsonArray pointsArray = jsonObj.value("points").toArray();
            QString color = jsonObj.value("color").toString();

            // Проверка на наличие необходимых данных
            if (pointsArray.size() != 6) {
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
            qDebug() << "Получена линия с точками:" << x1 << y1 << z1 << x2 << y2 << z2 << "и цветом:" << color;

            emit addLine(x1, y1, z1, x2, y2, z2, QColor(color));

            // Вызов функции для обработки полилинии
            // processPolyline(x1, y1, x2, y2, color);
        }

        // CUBE
        else if(type == "cube") {
            if(jsonObj.size() != 4) {
                qDebug() << "Недостаточно параметров для куба.";
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("size")) {
                qDebug() << "Не те параметры для куба.";
                return;
            }
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

        // SPHERE
        else if(type == "sphere") {
            if(jsonObj.size() != 4) {
                qDebug() << "Недостаточно параметров для сферы.";
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("radius")) {
                qDebug() << "Не те параметры для сферы.";
                return;
            }

            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double radius = jsonObj.value("radius").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                qDebug() << "Недостаточно точек для центра сферы.";
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            qDebug() << "Получен sphere с центром:" << x << y << z << "радиусом:" << radius << "и цветом:" << color;

            emit addSphere(x, y, z, radius, QColor(color));
        }

        // PLANE
        else if(type == "plane") {
            if(jsonObj.size() != 5) {
                qDebug() << "Недостаточно параметров для плоскости.";
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("width") || !jsonObj.contains("height")) {
                qDebug() << "Не те параметры для плоскости.";
                return;
            }

            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double width = jsonObj.value("width").toDouble();
            double height = jsonObj.value("height").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                qDebug() << "Недостаточно точек для центра плоскости.";
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            qDebug() << "Получен plane с центром:" << x << y << z << "шириной:" << width << "высотой:" << height << "и цветом:" << color;

            emit addPlane(x, y, z, width, height, QColor(color));
        }
        else {
            qDebug() << "Неизвестный тип сообщения:" << type;
        }
    }
    else {
        qDebug() << "Не хватает аргументов:";
    }
}
