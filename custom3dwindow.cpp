#include "custom3dwindow.h"

Custom3dWindow::Custom3dWindow(Qt3DCore::QEntity *root, QScreen *screen, Qt3DRender::API) : Qt3DExtras::Qt3DWindow(screen, Qt3DRender::API::OpenGL)
{
    this->setRootEntity(root);

    this->setTitle(QStringLiteral("3D Text CPP"));
    // this->defaultFrameGraph()->setClearColor(QColor(210, 210, 220));
    this->defaultFrameGraph()->setClearColor(QColor(0, 0, 0));

    // Install picking settings (for mouse click)
    this->renderSettings()->pickingSettings()->
        setFaceOrientationPickingMode(Qt3DRender::QPickingSettings::FrontAndBackFace);
    this->renderSettings()->pickingSettings()->
        setPickMethod(Qt3DRender::QPickingSettings::LinePicking);
    this->renderSettings()->pickingSettings()->
        setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);
    this->renderSettings()->pickingSettings()->
        setWorldSpaceTolerance(.01);

    // camera
    float aspect = static_cast<float>(this->screen()->size().width()) / this->screen()->size().height();
    Qt3DRender::QCamera *camera = this->camera();
    camera->lens()->setPerspectiveProjection(65.f, aspect, 0.1f, 1000.f);
    // camera->setPosition(QVector3D(0, 5, 3));
    // camera->setPosition(QVector3D(1.65889, 1.40765, 1.65179));
    camera->setPosition(QVector3D(30.1302, 25.5669, 30.0011));
    camera->setViewCenter(QVector3D(0, 0, 0));

    m_cameraController = new Qt3DExtras::QFirstPersonCameraController(root);
    m_cameraController->setCamera(camera);

    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);
}

void Custom3dWindow::setCameraSettings(const CameraSettings &newCameraSettings)
{
    m_cameraSettings = newCameraSettings;

    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);

    Qt3DRender::QCamera *camera = this->camera();
    camera->setFieldOfView(newCameraSettings.fieldOfView);
    camera->setNearPlane(newCameraSettings.nearPlane);
    camera->setFarPlane(newCameraSettings.farPlane);
}
