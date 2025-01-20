#include "worker.h"

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    auto *root = new Qt3DCore::QEntity();

    Qt3DExtras::Qt3DWindow *view = new Custom3dWindow(root);

    // Install picking settings (for mouse click)
    view->renderSettings()->pickingSettings()->
        setFaceOrientationPickingMode(Qt3DRender::QPickingSettings::FrontAndBackFace);
    view->renderSettings()->pickingSettings()->
        setPickMethod(Qt3DRender::QPickingSettings::LinePicking);
    view->renderSettings()->pickingSettings()->
        setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);
    view->renderSettings()->pickingSettings()->
        setWorldSpaceTolerance(.01);

    // { // plane
        // auto *plane = new Qt3DCore::QEntity(root);
        // auto *planeMesh = new Qt3DExtras::QPlaneMesh();
        // auto *planeTransform = new Qt3DCore::QTransform();
        // auto *planeMaterial = new Qt3DExtras::QPhongMaterial(root);
        // planeMesh->setWidth(10); planeMesh->setHeight(10);
        // planeTransform->setTranslation(QVector3D(0, 0, 0));
        // planeMaterial->setDiffuse(QColor(150, 150, 150));

        // plane->addComponent(planeMaterial);
        // plane->addComponent(planeMesh);
        // plane->addComponent(planeTransform);




        // auto *mouseHandler = new LineMouseHandler(plane);
        // plane->addComponent(mouseHandler);
    // }

    // auto *textMaterial = new Qt3DExtras::QPhongMaterial(root);
    // { // text
    //     int i = 0;
    //     const QStringList fonts = QFontDatabase().families();

    //     for (const QString &family : fonts)
    //     {
    //         auto *text = new Qt3DCore::QEntity(root);
    //         auto *textMesh = new Qt3DExtras::QExtrudedTextMesh();

    //         auto *textTransform = new Qt3DCore::QTransform();
    //         QFont font(family, 32, -1, false);
    //         textTransform->setTranslation(QVector3D(-2.45f, i * .5f, 0));
    //         textTransform->setScale(.2f);
    //         textMesh->setDepth(.45f);
    //         textMesh->setFont(font);
    //         textMesh->setText(family);
    //         textMaterial->setDiffuse(QColor(111, 150, 255));

    //         text->addComponent(textMaterial);
    //         text->addComponent(textMesh);
    //         text->addComponent(textTransform);

    //         i++;
    //     }
    // }


    drawLine({ 0, 0, 0 }, { 1, 0, 0 }, Qt::red, root); // X
    drawLine({ 0, 0, 0 }, { 0, 1, 0 }, Qt::green, root); // Y
    drawLine({ 0, 0, 0 }, { 0, 0, 1 }, Qt::blue, root); // Z

    // LineEntity *line1 = new LineEntity({ 0, 0, 0 }, { 1, 0, 0 }, Qt::red, "X", root); // X
    // LineEntity *line2 = new LineEntity({ 0, 0, 0 }, { 0, 1, 0 }, Qt::green, "Y", root); // Y
    // LineEntity *line3 = new LineEntity({ 0, 0, 0 }, { 0, 0, 1 }, Qt::blue, "Z", root); // Z

    // int size = 20;
    // // Рисуем линии по оси X и Y
    // for (int i = 0; i <= size; ++i) {
    //     // Линии по оси X (Y фиксирован)
    //     LineEntity *line = new LineEntity({0, static_cast<float>(i), 0}, {static_cast<float>(size), static_cast<float>(i), 0}, Qt::red, "X", root);
    //     // Линии по оси Y (X фиксирован)
    //     LineEntity *line1 = new LineEntity({static_cast<float>(i), 0, 0}, {static_cast<float>(i), static_cast<float>(size), 0}, Qt::red, "X", root);
    //     // Линии по оси Z (X фиксирован)

    //     LineEntity *line2 = new LineEntity({0, 0, static_cast<float>(i)}, {static_cast<float>(size), 0, static_cast<float>(i)}, Qt::blue, "Y", root);
    //     LineEntity *line3 = new LineEntity({static_cast<float>(i), 0, 0}, {static_cast<float>(i), 0, static_cast<float>(size)}, Qt::blue, "Y", root);

    //     // Линии по оси Z (Y фиксирован)
    //     LineEntity *line4 = new LineEntity({0, static_cast<float>(i), 0}, {0, static_cast<float>(i), static_cast<float>(size)}, Qt::green, "Z", root);
    //     LineEntity *line5 = new LineEntity({0, 0, static_cast<float>(i)}, {0, static_cast<float>(size), static_cast<float>(i)}, Qt::green, "Z", root);

    //     m_viewElements.append(line);
    //     m_viewElements.append(line1);
    //     m_viewElements.append(line2);
    //     m_viewElements.append(line3);
    //     m_viewElements.append(line4);
    //     m_viewElements.append(line5);
    // }

    // for(int i = 0; i < 20; i++) {
    //     Qt3DCore::QEntity *entity = Drawing3d::drawSimpleCube(QVector3D(i + 0.5, 0.5, i + 0.5), 1.0f, root);
    //     m_viewElements.append(entity);
    // }

    // qDebug() << static_cast<LineEntity*>(m_viewElements.at(0))->start() << static_cast<LineEntity*>(m_viewElements.at(0))->end();

    view->show();
}

void Worker::drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity)
{
    auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

    // position vertices (start and end)
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x();
    *positions++ = start.y();
    *positions++ = start.z();
    *positions++ = end.x();
    *positions++ = end.y();
    *positions++ = end.z();

    auto *buf = new Qt3DRender::QBuffer(geometry);
    buf->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int)); // start to end
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

    // mesh
    auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
    material->setAmbient(color);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}
