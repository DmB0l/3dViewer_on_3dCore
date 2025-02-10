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
    // m_camera->setPosition(QVector3D(30.1302, 25.5669, 30.0011));
    m_camera->setPosition(QVector3D(100, 100, 100));
    m_camera->setViewCenter(QVector3D(0, 0, 0));

    m_cameraController = m_rootObject->findChild<Qt3DExtras::QFirstPersonCameraController *>("cameraController");
    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);

    setCameraPosition(m_camera->position());

    connect(m_camera, &Qt3DRender::QCamera::positionChanged, this, [=](const QVector3D &position){
        setCameraPosition(position);
    });
}

Qt3DCore::QEntity *View3d::rootEntity() const {
    return m_rootEntity;
}

void View3d::setColorSettings(const QColor &color) {
    m_backgroundColor = color;
    m_rootEntity->componentsOfType<Qt3DRender::QRenderSettings>().at(0)->activeFrameGraph()->setProperty("clearColor", m_backgroundColor);
}

void View3d::setCameraSettings(const CameraSettings &newCameraSettings) {
    m_cameraSettings = newCameraSettings;

    m_cameraController->setLinearSpeed(m_cameraSettings.linearSpeed);
    m_cameraController->setLookSpeed(m_cameraSettings.lookSpeed);

    m_camera->setFieldOfView(newCameraSettings.fieldOfView);
    m_camera->setNearPlane(newCameraSettings.nearPlane);
    m_camera->setFarPlane(newCameraSettings.farPlane);
}

void View3d::createSelectedEntityText(Qt3DCore::QEntity *entity) {
    QString selectedEntityText;
    if(entity->objectName() == "line") {
        float *positions = reinterpret_cast<float*>(entity->componentsOfType<Qt3DRender::QGeometryRenderer>().at(0)->geometry()->attributes().at(0)->buffer()->data().data());
        QVector3D startPosition(*positions, *(positions + 1), *(positions + 2));
        QVector3D endPosition(*(positions + 3), *(positions + 4), *(positions + 5));

        selectedEntityText.append("Selected ").
            append(entity->objectName()).append(" start { ").
            append(QString::number(startPosition.x())).append("; ").
            append(QString::number(startPosition.y())).append("; ").
            append(QString::number(startPosition.z())).append(" ").
            append("}\n").
            append("                           end  { ").
            append(QString::number(endPosition.x())).append("; ").
            append(QString::number(endPosition.y())).append("; ").
            append(QString::number(endPosition.z())).append(" ").
            append("}");
    }
    else {
        QVector3D position = entity->componentsOfType<Qt3DCore::QTransform>().at(0)->translation();
        selectedEntityText.append("Selected ").
            append(entity->objectName()).append(" { ").
            append(QString::number(position.x())).append("; ").
            append(QString::number(position.y())).append("; ").
            append(QString::number(position.z())).append(" ").
            append("}");
    }
    setSelectedEntityText(selectedEntityText);
}

void View3d::clearSelectedEntityText() {
    setSelectedEntityText("");
}

QVector3D View3d::cameraPosition() const {
    return m_cameraPosition;
}

void View3d::setCameraPosition(const QVector3D &newCameraPosition) {
    if (m_cameraPosition == newCameraPosition)
        return;
    m_cameraPosition = newCameraPosition;
    emit cameraPositionChanged();
}

QString View3d::selectedEntityText() const {
    return m_selectedEntityText;
}

void View3d::setSelectedEntityText(const QString &newSelectedEntityText) {
    if (m_selectedEntityText == newSelectedEntityText)
        return;
    m_selectedEntityText = newSelectedEntityText;
    emit selectedEntityTextChanged();
}

QString View3d::portText() const
{
    return m_portText;
}

void View3d::setPortText(const QString &newPortText)
{
    if (m_portText == newPortText)
        return;
    m_portText = newPortText;
    emit portTextChanged();
}
