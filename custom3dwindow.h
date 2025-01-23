#ifndef CUSTOM3DWINDOW_H
#define CUSTOM3DWINDOW_H

#include <QObject>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DExtras/QExtrudedTextMesh>

#include <QtMath>

#include "settings.h"

class Custom3dWindow : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    Custom3dWindow(Qt3DCore::QEntity *root = nullptr, QScreen *screen = nullptr, Qt3DRender::API = Qt3DRender::API::OpenGL);

    void setCameraSettings(const CameraSettings &newCameraSettings);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        qDebug() << this->camera()->position();
        if (event->button() == Qt::LeftButton) {
            // m_cameraController->setLinearSpeed(-m_linearSpeed);
            // m_cameraController->setLookSpeed(-m_LookSpeed);
        }
        // else if(event->button() == Qt::RightButton) {
        //     QVector3D viewCenter = this->camera()->position();
        //     viewCenter.setX(viewCenter.x() - 1);
        //     viewCenter.setY(viewCenter.y() - 1);
        //     viewCenter.setZ(viewCenter.z() - 1);
        //     this->camera()->setViewCenter(viewCenter);
        // }
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        // m_cameraController->setLinearSpeed(m_linearSpeed);
        // m_cameraController->setLookSpeed(m_LookSpeed);
    }

private:
    Qt3DExtras::QFirstPersonCameraController *m_cameraController;
    CameraSettings m_cameraSettings;
};

#endif // CUSTOM3DWINDOW_H
