#ifndef WORKER_H
#define WORKER_H

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
#include <Qt3DRender/QObjectPicker>

#include <QSharedPointer>

#include "custom3dwindow.h"
#include "lineentity.h"
#include "linemousehandler.h"
#include "drawing3d.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void drawLine(const QVector3D &start, const QVector3D &end, const QColor &color, Qt3DCore::QEntity *_rootEntity);
signals:

private:
    QVector<Qt3DCore::QEntity *> m_viewElements;
    QVector<Qt3DCore::QEntity *> m_grid;

    int m_counter = 0;
};

#endif // WORKER_H
