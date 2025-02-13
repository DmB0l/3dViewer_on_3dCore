#include "explosioneffect.h"


ExplosionEffect::ExplosionEffect(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent) {

    spdlog::info("попытка создания и отрисовки взрыва");

    // Создаем сферу для эффекта взрыва
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRadius(0.5f);

    // Материал взрыва
    auto material = new Qt3DExtras::QPhongMaterial(this);
    material->setDiffuse(QColor(Qt::yellow));
    material->setAmbient(QColor(Qt::red));

    // Трансформация
    auto transform = new Qt3DCore::QTransform(this);

    // Добавляем компоненты
    addComponent(sphereMesh);
    addComponent(material);
    addComponent(transform);

    // Анимация масштаба
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(transform);
    scaleAnimation->setTargetObject(transform);
    scaleAnimation->setPropertyName("scale");
    scaleAnimation->setStartValue(1.0f);
    scaleAnimation->setEndValue(10.0f);
    scaleAnimation->setDuration(1000); // 0.5 секунды
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // Анимация прозрачности
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(material);
    opacityAnimation->setTargetObject(material);
    opacityAnimation->setPropertyName("ambient");
    opacityAnimation->setStartValue(QColor(Qt::red));
    opacityAnimation->setEndValue(QColor(Qt::transparent));
    opacityAnimation->setDuration(1000); // 0.5 секунды
    opacityAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // Запускаем анимацию
    scaleAnimation->start();
    opacityAnimation->start();

    // Удаляем эффект после завершения анимации
    connect(scaleAnimation, &QPropertyAnimation::finished, this, [this]() {
        deleteLater();
    });

    spdlog::info("создание и отрисовка взрыва успешно");
}

void ExplosionEffect::setPosition(const QVector3D &position) {
    auto transform = componentsOfType<Qt3DCore::QTransform>().at(0);
    if (transform) {
        transform->setTranslation(position);
    }
}
