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
public:
    View3d(QQuickWidget *quickWidget, QObject *parent = nullptr);
    explicit View3d(QObject *parent = nullptr);

    Qt3DCore::QEntity *rootEntity() const;

    void setCameraSettings(const CameraSettings &newCameraSettings);

private:
    QQuickWidget *m_quickWidget;
    QObject *m_rootObject;
    Qt3DCore::QEntity *m_rootEntity;

    Qt3DRender::QCamera *m_camera;
    Qt3DExtras::QFirstPersonCameraController *m_cameraController;
    CameraSettings m_cameraSettings;
};

#endif // VIEW3D_H
