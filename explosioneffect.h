#ifndef EXPLOSIONEFFECT_H
#define EXPLOSIONEFFECT_H

#include <QObject>
#include <QGuiApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DCore/QTransform>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVector3D>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class ExplosionEffect : public Qt3DCore::QEntity {
public:
    explicit ExplosionEffect(Qt3DCore::QNode *parent = nullptr);

    void setPosition(const QVector3D &position);
};

#endif // EXPLOSIONEFFECT_H
