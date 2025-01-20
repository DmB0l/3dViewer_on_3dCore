#include "lineentity.h"


LineEntity::LineEntity(const QVector3D &start, const QVector3D &end, const QColor &color, const QString name, Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent), m_start(start), m_end(end)
{
    this->setObjectName(name);

    //Set event on clicking mouse
    // auto *picker = new Qt3DRender::QObjectPicker(this);
    // picker->setHoverEnabled(false);
    // picker->setEnabled(true);
    // QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, &LineEntity::onLineClicked);
    // this->addComponent(picker);

    drawLine(start, end, color, this);
}

void LineEntity::onLineClicked(Qt3DRender::QPickEvent *evt) {
    if(evt->button() == Qt3DRender::QPickEvent::LeftButton) {
        Qt3DCore::QEntity *entity = qobject_cast<Qt3DCore::QEntity*>(sender()->parent());
        qDebug() << "Picked" << entity->objectName();

        auto *textMaterial = new Qt3DExtras::QPhongMaterial(this);
        { // text
            int i = 0;
            auto *text = new Qt3DCore::QEntity(this);
            auto *textMesh = new Qt3DExtras::QExtrudedTextMesh();

            auto *textTransform = new Qt3DCore::QTransform();
            QFont font("Times", 32, -1, false);
            textTransform->setTranslation(m_end);
            textTransform->setScale(.1f);
            textMesh->setDepth(.1f);
            textMesh->setFont(font);
            textMesh->setText(QString::number(m_end.x()) + ", " + QString::number(m_end.y()) + ", " + QString::number(m_end.z()));
            textMaterial->setDiffuse(QColor(111, 150, 255));

            text->addComponent(textMaterial);
            text->addComponent(textMesh);
            text->addComponent(textTransform);

            i++;
        }
    }
}

QVector3D LineEntity::start() const
{
    return m_start;
}

QVector3D LineEntity::end() const
{
    return m_end;
}

void LineEntity::drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity)
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
    // auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    this->addComponent(line);
    this->addComponent(material);
}
