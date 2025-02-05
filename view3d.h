#ifndef VIEW3D_H
#define VIEW3D_H

#include <Qt3DCore/Qt3DCore>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DExtras/QExtrudedTextMesh>

#include <QtMath>

#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QQmlListReference>

#include "settings.h"

class View3d : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(double m_cameraPositionX READ getCameraPositionX WRITE setCameraPositionX NOTIFY cameraPositionChanged)
    Q_PROPERTY(QVector3D m_cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged FINAL)
    Q_PROPERTY(QString m_selectedEntityText READ selectedEntityText WRITE setSelectedEntityText NOTIFY selectedEntityTextChanged FINAL)
    Q_PROPERTY(QString m_portText READ portText WRITE setPortText NOTIFY portTextChanged FINAL)

public:
    View3d(QQuickWidget *quickWidget, QObject *parent = nullptr);
    explicit View3d(QObject *parent = nullptr);

    Qt3DCore::QEntity *rootEntity() const;

    void setCameraSettings(const CameraSettings &newCameraSettings);
    void setColorSettings(const QColor &color);

    void createSelectedEntityText(Qt3DCore::QEntity *entity);
    void clearSelectedEntityText();

    QVector3D cameraPosition() const;
    void setCameraPosition(const QVector3D &newCameraPosition);
    QString selectedEntityText() const;
    void setSelectedEntityText(const QString &newSelectedEntityText);
    QString portText() const;
    void setPortText(const QString &newPortText);

    // Q_INVOKABLE
        // void updatePort(QString port);


signals:
    //QML
    void cameraPositionChanged();
    void selectedEntityTextChanged();
    void portTextChanged();

    //QT
    void updatePortSettings(QString port);

private:
    QQuickWidget *m_quickWidget;
    QObject *m_rootObject;
    Qt3DCore::QEntity *m_rootEntity;

    Qt3DRender::QCamera *m_camera;
    Qt3DExtras::QFirstPersonCameraController *m_cameraController;
    CameraSettings m_cameraSettings;

    // double m_cameraPositionX = 0;
    QVector3D m_cameraPosition;
    QString m_selectedEntityText;
    QString m_portText;

    QColor m_backgroundColor;
};

#endif // VIEW3D_H
