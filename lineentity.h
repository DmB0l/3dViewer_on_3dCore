#ifndef LINEENTITY_H
#define LINEENTITY_H

#include <QObject>
#include <QCoreApplication>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DExtras/QExtrudedTextMesh>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>
#include <QInputAspect>

// #include "linemousehandler.h"

class LineEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    LineEntity(const QVector3D& start, const QVector3D& end, const QColor& color, const QString name, Qt3DCore::QEntity* parent = nullptr);

    void drawLine(const QVector3D &start, const QVector3D &end, const QColor &color, Qt3DCore::QEntity *_rootEntity);

    QVector3D start() const;
    QVector3D end() const;

private slots:
    void onLineClicked(Qt3DRender::QPickEvent *evt);

private:
    QVector3D m_start;
    QVector3D m_end;
    // LineMouseHandler *m_mouseHandler;
};

#endif // LINEENTITY_H
