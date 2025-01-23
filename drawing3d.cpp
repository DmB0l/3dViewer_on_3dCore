#include "drawing3d.h"

Drawing3d::Drawing3d(QObject *parent) : QObject(parent)
{}

Qt3DCore::QEntity* Drawing3d::drawPlane(QVector3D pos, double width, double height, QColor color, Qt3DCore::QEntity *root) {
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(width);
    planeMesh->setHeight(height);

    auto *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setTranslation(pos);

    auto *planeMaterial = new Qt3DExtras::QPhongMaterial(root);
    planeMaterial->setDiffuse(color);

    Qt3DCore::QEntity *planeEntity = new Qt3DCore::QEntity(root);
    planeEntity->addComponent(planeMaterial);
    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeTransform);

    planeEntity->setObjectName("plane");

    return planeEntity;
}

Qt3DCore::QEntity* Drawing3d::drawCube(QVector3D pos, double size, QColor color, Qt3DCore::QEntity *root) {
    // Cuboid shape data
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();

    Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(size);
    cuboidTransform->setTranslation(pos);

    Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    // cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));
    cuboidMaterial->setDiffuse(color);

    //Cuboid
    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(root);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(cuboidMaterial);
    cuboidEntity->addComponent(cuboidTransform);

    cuboidEntity->setObjectName("cube");

    return cuboidEntity;
}

Qt3DCore::QEntity* Drawing3d::drawSphere(QVector3D pos, double radius, QColor color, Qt3DCore::QEntity *root) {
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRadius(radius);

    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(pos);

    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(root);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(material);
    sphereEntity->addComponent(transform);

    sphereEntity->setObjectName("sphere");

    return sphereEntity;
}

Qt3DCore::QEntity* Drawing3d::drawLine(double x1, double y1, double z1,
                                       double x2, double y2, double z2,
                                       QColor color, Qt3DCore::QEntity *root)
{
    return new LineEntity({static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(z1)},
                          {static_cast<float>(x2), static_cast<float>(y2), static_cast<float>(z2)},
                          color, "line", root);
}

QVector<LineEntity*> Drawing3d::createGrid(double minX, double minY, double minZ,
                                            double maxX, double maxY, double maxZ,
                                            uint32_t segmentCountX,
                                            uint32_t segmentCountY,
                                            uint32_t segmentCountZ,
                                            Qt3DCore::QEntity *root)
{
    QVector<LineEntity*> lines;


    for (float i = minX; i <= maxX; i += segmentCountX /*/ (abs(minX) + abs(maxX))*/) {
        LineEntity *line1 = new LineEntity({static_cast<float>(i), static_cast<float>(minY), 0},
                                           {static_cast<float>(i), static_cast<float>(maxY), 0},
                                           Qt::red, "X", root);

        LineEntity *line2 = new LineEntity({static_cast<float>(i), 0, static_cast<float>(minZ)},
                                           {static_cast<float>(i), 0, static_cast<float>(maxZ)},
                                           Qt::green, "Z", root);

        lines.append(line1);
        lines.append(line2);
    }


    for (float i = minY; i <= maxY; i += segmentCountY /*/ (abs(minX) + abs(maxX))*/) {
        LineEntity *line1 = new LineEntity({static_cast<float>(minX), static_cast<float>(i), 0},
                                           {static_cast<float>(maxX), static_cast<float>(i), 0},
                                           Qt::red, "X", root);

        LineEntity *line2 = new LineEntity({0, static_cast<float>(i), static_cast<float>(minZ)},
                                           {0, static_cast<float>(i), static_cast<float>(maxZ)},
                                           Qt::blue, "Y", root);

        lines.append(line1);
        lines.append(line2);
    }

    for (float i = minZ; i <= maxZ; i += segmentCountZ /*/ (abs(minX) + abs(maxX))*/) {
        LineEntity *line1 = new LineEntity({static_cast<float>(minX), 0, static_cast<float>(i)},
                                           {static_cast<float>(maxX), 0, static_cast<float>(i)},
                                           Qt::green, "Z", root);

        LineEntity *line2 = new LineEntity({0, static_cast<float>(minY), static_cast<float>(i)},
                                           {0, static_cast<float>(maxY), static_cast<float>(i)},
                                           Qt::blue, "Y", root);

        lines.append(line1);
        lines.append(line2);
    }

    return lines;
}

QVector<Qt3DCore::QEntity *> Drawing3d::createHeart(Qt3DCore::QEntity *parentEntity, QVector3D pos) {
    QVector<Qt3DCore::QEntity *> heart;

    const int numPoints = 100;
    for (int i = 0; i < numPoints; ++i) {
        double u = (i / (double)(numPoints - 1)) * 2 * M_PI; // u from 0 to 2π
        for (int v = -10; v <= 10; v++) { // v from -1 to 1
            double x = 16 * pow(sin(u), 3);
            double y = 13 * cos(u) - 5 * cos(2 * u) - 2 * cos(3 * u) - cos(4 * u);
            double z = (double)v / 10.0;

            heart.append(this->drawSphere(QVector3D(pos.x() + x, pos.y() + y, pos.z() + z), 0.5f, QColor(Qt::red), parentEntity));
        }
    }

    return heart;
}

Qt3DCore::QEntity* Drawing3d::createStar(Qt3DCore::QEntity *parent, const QVector3D &position, float size) {
    // Create a new star entity
    Qt3DCore::QEntity *starEntity = new Qt3DCore::QEntity(parent);

    // Create a sphere mesh for the star
    Qt3DExtras::QSphereMesh *starMesh = new Qt3DExtras::QSphereMesh();
    starMesh->setRadius(size); // Set the size of the star

    // Create a transform for the star
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(position); // Set the position of the star

    // Create a material for the star
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(255, 255, 255)); // Set color to white

    // Add components to the star entity
    starEntity->addComponent(starMesh);
    starEntity->addComponent(transform);
    starEntity->addComponent(material);

    return starEntity;
}

QVector<Qt3DCore::QEntity*> Drawing3d::createStarrySky(Qt3DCore::QEntity *rootEntity, int starCount,
                                                        double minX, double minY, double minZ,
                                                        double maxX, double maxY, double maxZ)
{
    QVector<Qt3DCore::QEntity*> stars;

    QRandomGenerator *gen = QRandomGenerator::global();
    for(int j = 0; j < 3; j++) {
        for (int i = 0; i < starCount / 3; ++i) {
            // Generate random position and size for each star

            int posX = gen->bounded(0, 2);
            int posY = gen->bounded(0, 2);
            int posZ = gen->bounded(0, 2);

            int coordX = 0;
            int coordY = 0;
            int coordZ = 0;

            if(j == 0) {
                if(posX == 0) {
                    coordX = gen->bounded((int)minX - 110, (int)minX - 10);
                }
                else if(posX == 1) {
                    coordX = gen->bounded((int)maxX + 10, (int)maxX + 110);
                }
                coordY = gen->bounded((int)minY - 110, (int)maxY + 110);
                coordZ = gen->bounded((int)minZ - 110, (int)maxZ + 110);
            }
            else if(j == 1) {
                if(posY == 0) {
                    coordY = gen->bounded((int)minY - 110, (int)minY - 10);
                }
                else if(posY == 1) {
                    coordY = gen->bounded((int)maxY + 10, (int)maxY + 110);
                }
                coordX = gen->bounded((int)minX - 110, (int)maxX + 110);
                coordZ = gen->bounded((int)minZ - 110, (int)maxZ + 110);
            }
            else if(j == 2) {
                if(posZ == 0) {
                    coordZ = gen->bounded((int)minZ - 110, (int)minZ - 10);
                }
                else if(posZ == 1) {
                    coordZ = gen->bounded((int)maxZ + 10, (int)maxZ + 110);
                }
                coordX = gen->bounded((int)minX - 110, (int)maxX + 110);
                coordY = gen->bounded((int)minY - 110, (int)maxY + 110);
            }

            QVector3D position(
                // gen->bounded(-100, 100), // Random x
                // gen->bounded(-100, 100), // Random y
                // gen->bounded(-100, 100)  // Random z
                coordX,
                coordY,
                coordZ
                );
            float randomValue = gen->generateDouble(); // Возвращает значение от 0.0 до 1.0
            // Масштабируем его в диапазон от 0.05 до 0.2
            float size = 0.05 + randomValue * (0.2 - 0.05);

            // Create the star
            stars.append(createStar(rootEntity, position, size));
        }
    }
    // emit starsSkyUpdate(stars);
    return stars;
}
