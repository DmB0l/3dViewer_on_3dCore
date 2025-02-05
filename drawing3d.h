#ifndef DRAWING3D_H
#define DRAWING3D_H

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
#include <Qt3DRender/QSceneLoader>

#include "lineentity.h"

class Drawing3d : public QObject
{
    Q_OBJECT
public:
    Drawing3d(QObject *parent = nullptr);

public slots:
    Qt3DCore::QEntity *drawPlane(QVector3D pos, double width, double height, QColor color, Qt3DCore::QEntity *root);

    Qt3DCore::QEntity* drawSphere(QVector3D pos, double radius, QColor color, Qt3DCore::QEntity *root);

    Qt3DCore::QEntity* drawCube(QVector3D pos, double size, QColor color, Qt3DCore::QEntity *root);

    Qt3DCore::QEntity *drawObj(QString filePath, QVector3D pos, QColor color, double scale, Qt3DCore::QEntity *root);

    QVector<Qt3DCore::QEntity *> createGrid(double minX = 0, double minY = 0, double minZ = 0,
                                            double maxX = 20, double maxY = 20, double m_maxZ = 20,
                                            uint32_t segmentCountX = 1,
                                            uint32_t segmentCountY = 1,
                                            uint32_t segmentCountZ = 1,
                                            Qt3DCore::QEntity *root = nullptr);

    QVector<Qt3DCore::QEntity *> createHeart(Qt3DCore::QEntity *parentEntity, QVector3D pos);

    QVector<Qt3DCore::QEntity*> createStarrySky(Qt3DCore::QEntity *rootEntity, int starCount,
                                double minX, double minY, double minZ,
                                double maxX, double maxY, double maxZ);

    Qt3DCore::QEntity *drawLine(double x1, double y1, double z1, double x2, double y2, double z2, QColor color, QString name, Qt3DCore::QEntity *root);

private:
    Qt3DCore::QEntity* createStar(Qt3DCore::QEntity *parent, const QVector3D &position, float size);

signals:
    void entityClicked(Qt3DCore::QEntity *entity, QColor prevColor);

};

#endif // DRAWING3D_H
