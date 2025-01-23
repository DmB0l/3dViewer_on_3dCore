#include "view3d.h"

View3d::View3d(QObject *parent) : QObject{parent}
{}

View3d::View3d(QQuickWidget *quickWidget, QObject *parent)
    : QObject{parent},
    m_quickWidget{quickWidget}
{
    m_quickWidget->rootContext()->setContextProperty("View3Dcpp", this);
    m_quickWidget->setSource(QUrl(QStringLiteral("qrc:/qml/View3d.qml")));

    m_rootObject = m_quickWidget->rootObject();
    m_rootEntity = m_rootObject->findChild<Qt3DCore::QEntity *>("rootEntity");
    qDebug() << m_rootEntity->objectName();

    // camera
    m_camera = m_rootObject->findChild<Qt3DRender::QCamera *>("camera");
    float aspect = static_cast<float>(quickWidget->screen()->size().width()) / quickWidget->screen()->size().height();
    m_camera->lens()->setPerspectiveProjection(65.f, aspect, 0.1f, 1000.f);
    m_camera->setPosition(QVector3D(30.1302, 25.5669, 30.0011));
    m_camera->setViewCenter(QVector3D(0, 0, 0));

    m_cameraController = m_rootObject->findChild<Qt3DExtras::QFirstPersonCameraController *>("cameraController");
    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);
}

Qt3DCore::QEntity *View3d::rootEntity() const
{
    return m_rootEntity;
}

void View3d::setCameraSettings(const CameraSettings &newCameraSettings)
{
    m_cameraSettings = newCameraSettings;

    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);

    m_camera->setFieldOfView(newCameraSettings.fieldOfView);
    m_camera->setNearPlane(newCameraSettings.nearPlane);
    m_camera->setFarPlane(newCameraSettings.farPlane);
}
