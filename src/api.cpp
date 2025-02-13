#include "api.h"

API::API(QObject *parent)
    : QObject{parent},
    m_listenSocket(new QUdpSocket)
{
    loadSettings();
    updateBindSocket();
}

void API::readPendingDatagrams()
{
    QByteArray buffer;
    buffer.resize(m_listenSocket->pendingDatagramSize());
    qint64 bytesRead = m_listenSocket->readDatagram(buffer.data(), buffer.size());
    if (bytesRead < 0) {
        spdlog::error("Ошибка чтения датаграммы: {}", m_listenSocket->errorString().toStdString());
    }

    // Парсинг JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(buffer);
    if (jsonDoc.isNull()) {
        spdlog::error("Ошибка парсинга JSON: " + buffer);
        return;
    }

    if (!jsonDoc.isObject()) {
        spdlog::error("Ожидался объект JSON, но получен другой тип.");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if(jsonObj.size() >= 1) {
        // LINE
        QString type = jsonObj.value("type").toString();
        if (type == "line") {
            if(jsonObj.size() != 3) {
                spdlog::error("Недостаточно параметров для линии.");
                return;
            }
            if(!jsonObj.contains("points") || !jsonObj.contains("color")) {
                spdlog::error("Не те параметры для линии.");
                return;
            }

            QJsonArray pointsArray = jsonObj.value("points").toArray();
            QString color = jsonObj.value("color").toString();

            // Проверка на наличие необходимых данных
            if (pointsArray.size() != 6) {
                spdlog::error("Недостаточно точек для линии.");
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
            spdlog::info("Получена линия с точками: (1): {}, {}, {};  (2): {}, {}, {}   color: {}",
                         x1, y1, z1, x2, y2, z2, color.toStdString());

            emit addLine(x1, y1, z1, x2, y2, z2, QColor(color));

            // Вызов функции для обработки полилинии
            // processPolyline(x1, y1, x2, y2, color);
        }

        // CUBE
        else if(type == "cube") {
            if(jsonObj.size() != 4) {
                spdlog::error("Недостаточно параметров для куба.");
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("size")) {
                spdlog::error("Не те параметры для куба.");
                return;
            }
            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double size = jsonObj.value("size").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра куба.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен cube с центром: {}, {}, {};   size: {};   color: {}", x, y, z, size, color.toStdString());

            emit addCube(x, y, z, size, QColor(color));
        }

        // SPHERE
        else if(type == "sphere") {
            if(jsonObj.size() != 4) {
                spdlog::error("Недостаточно параметров для сферы.");
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("radius")) {
                spdlog::error("Не те параметры для сферы.");
                return;
            }

            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double radius = jsonObj.value("radius").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра сферы.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен sphere с центром: {}, {}, {};   radius: {};  color: {}", x, y, z, radius, color.toStdString());

            emit addSphere(x, y, z, radius, QColor(color));
        }

        // PLANE
        else if(type == "plane") {
            if(jsonObj.size() != 5) {
                spdlog::error("Недостаточно параметров для плоскости.");
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("color") || !jsonObj.contains("width") || !jsonObj.contains("height")) {
                spdlog::error("Не те параметры для плоскости.");
                return;
            }

            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double width = jsonObj.value("width").toDouble();
            double height = jsonObj.value("height").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра плоскости.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен plane с центром: {}, {}, {};   width: {};   height: {};   color: {};", x, y, z, width, height, color.toStdString());

            emit addPlane(x, y, z, width, height, QColor(color));
        }

        // TORUS
        else if(type == "torus") {
            if(jsonObj.size() != 8) {
                spdlog::error("Недостаточно параметров для torus.");
                return;
            }
            if(!jsonObj.contains("center") || !jsonObj.contains("radius") ||
                !jsonObj.contains("minorRadius") ||!jsonObj.contains("rings") ||
                !jsonObj.contains("rotationX") || !jsonObj.contains("rotationY") ||
                !jsonObj.contains("color"))
            {
                spdlog::error("Не те параметры для torus.");
                return;
            }

            QJsonArray centerArray = jsonObj.value("center").toArray();
            double radius = jsonObj.value("radius").toDouble();
            double minorRadius = jsonObj.value("minorRadius").toDouble();
            int rings = jsonObj.value("rings").toInt();
            double rotationX = jsonObj.value("rotationX").toDouble();
            double rotationY = jsonObj.value("rotationY").toDouble();
            QString color = jsonObj.value("color").toString();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра torus.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен torus с центром: {}, {}, {};   radius: {};   minor radius: {},   "
                         "rings: {},   rotationX: {},   rotationY: {};   color: {}",
                         x, y, z, radius, minorRadius, rings, rotationX, rotationY, color.toStdString());

            emit addTorus(x, y, z, radius, minorRadius, rings, rotationX, rotationY, QColor(color));
        }

        // OBJ
        else if(type == "obj") {
            if(jsonObj.size() != 7) {
                spdlog::error("Недостаточно параметров для объекта.");
                return;
            }
            if(!jsonObj.contains("path") || !jsonObj.contains("center") ||
                !jsonObj.contains("color") || !jsonObj.contains("scale") ||
                !jsonObj.contains("rotationX") || !jsonObj.contains("rotationY"))
            {
                spdlog::error("Не те параметры для объекта.");
                return;
            }

            QString path = jsonObj.value("path").toString();
            QJsonArray centerArray = jsonObj.value("center").toArray();
            QString color = jsonObj.value("color").toString();
            double scale = jsonObj.value("scale").toDouble();
            double rotationX = jsonObj.value("rotationX").toDouble();
            double rotationY = jsonObj.value("rotationY").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра объекта.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен obj с центром: {}, {}, {};   path: {};   scale: {};   color: {}",
                         x, y, z, path.toStdString(), scale, color.toStdString());

            if(!QFile::exists(path)) {
                spdlog::error("Файл {} не существует. Невозможно отрисовать объект", path.toStdString());
            }
            else {
                emit addObj(path, x, y, z, scale, rotationX, rotationY, QColor(color));
            }
        }

        // TEXTURE OBJ
        else if(type == "textureObj") {
            if(jsonObj.size() != 7) {
                spdlog::error("Недостаточно параметров для объекта с текстурой.");
                return;
            }
            if(!jsonObj.contains("objPath") || !jsonObj.contains("texturePath") ||
                !jsonObj.contains("center") || !jsonObj.contains("scale") ||
                !jsonObj.contains("rotationX") || !jsonObj.contains("rotationY"))
            {
                spdlog::error("Не те параметры для объекта с текстурой.");
                return;
            }

            QString objPath = jsonObj.value("objPath").toString();
            QString texturePath = jsonObj.value("texturePath").toString();
            QJsonArray centerArray = jsonObj.value("center").toArray();
            double scale = jsonObj.value("scale").toDouble();
            double rotationX = jsonObj.value("rotationX").toDouble();
            double rotationY = jsonObj.value("rotationY").toDouble();

            // Проверка на наличие необходимых данных
            if (centerArray.size() < 3) {
                spdlog::error("Недостаточно точек для центра объекта с текстурой.");
                return;
            }

            // Извлечение координат
            qreal x = centerArray[0].toDouble();
            qreal y = centerArray[1].toDouble();
            qreal z = centerArray[2].toDouble();

            // Здесь вы можете обработать полученные данные
            spdlog::info("Получен textureObj с центром: {}, {}, {};   object path: {};   texture path: {};   scale: {}",
                         x, y, z, objPath.toStdString(), texturePath.toStdString(), scale);

            if(!QFile::exists(texturePath)) {
                spdlog::error("Файл {} не существует. Невозможно отрисовать текстуру", texturePath.toStdString());
            }

            if(!QFile::exists(objPath)) {
                spdlog::error("Файл {} не существует. Невозможно отрисовать объект", objPath.toStdString());
            }
            else {
                emit addTextureObj(objPath, texturePath, x, y, z, scale, rotationX, rotationY);
            }
        }
        else if(type == "clearAll") {
            emit clearAll();

            spdlog::info("Получена полная очистка сцены");
        }
        else {
            spdlog::error("Неизвестный тип сообщения: {}", type.toStdString());
        }
    }
    else {
        spdlog::error("Не хватает аргументов");
    }
}

void API::saveSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("API PARAMETERS");
    settings.setValue("port", m_listenPort);
    settings.endGroup();
}

void API::loadSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    if(!settings.childGroups().contains("API PARAMETERS")) {
        saveSettings();
    }
    else {
        settings.beginGroup("API PARAMETERS");
        m_listenPort = settings.value("port").toInt();
        settings.endGroup();
    }
}

void API::updateBindSocket() {
    if (m_listenSocket->state() != QAbstractSocket::UnconnectedState) {
        m_listenSocket->abort();
    }

    spdlog::info("new port: {}", m_listenPort);
    m_listenSocket->bind(QHostAddress::Any, m_listenPort);
    connect(m_listenSocket, &QUdpSocket::readyRead, this, &API::readPendingDatagrams);
}

int API::listenPort() const
{
    return m_listenPort;
}

void API::setListenPort(const int &newListenPort)
{
    m_listenPort = newListenPort;
    updateBindSocket();
    saveSettings();
}


