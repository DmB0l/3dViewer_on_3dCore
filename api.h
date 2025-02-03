#ifndef API_H
#define API_H

#include <QObject>
#include <QUdpSocket>
#include <QDataStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSettings>

#include <QColor>

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = nullptr);

    int listenPort() const;

public slots:
    void readPendingDatagrams();
    void setListenPort(const int &newListenPort);

private:
    void updateBindSocket();
    void saveSettings();
    void loadSettings();

    QUdpSocket *m_listenSocket;
    int m_listenPort = 5831;
    // QString m_listenIp = "127.0.0.1";

signals:
    void addLine(double x1, double y1, double z1,
                 double x2, double y2, double z2,
                 QColor color);

    void addCube(double x, double y, double z,
                 double size,
                 QColor color);

    void addSphere(double x, double y, double z,
                   double radius,
                   QColor color);

    void addPlane(double x, double y, double z,
                  double width, double height,
                  QColor color);

    void clearAll();
};

#endif // API_H
