#include "drawing3d.h"

Drawing3d::Drawing3d(QObject *parent) : QObject(parent)
{}

Qt3DCore::QEntity* Drawing3d::drawPlane(QVector3D pos, double width, double height, QColor color, Qt3DCore::QEntity *root) {
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(width);
    planeMesh->setHeight(height);

    auto *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setTranslation(pos);

    auto *planeMaterial = new Qt3DExtras::QPhongMaterial();
    // planeMaterial->setDiffuse(color);
    planeMaterial->setAmbient(color);

    Qt3DCore::QEntity *planeEntity = new Qt3DCore::QEntity();
    planeEntity->addComponent(planeMaterial);
    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeTransform);

    planeEntity->setObjectName("plane");

    auto *picker = new Qt3DRender::QObjectPicker();
    picker->setHoverEnabled(true);
    picker->setEnabled(true);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, [=](){
        planeMaterial->setAmbient(QColor("yellow"));
        emit entityClicked(planeEntity, color);
    });
    planeEntity->addComponent(picker);

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
    // cuboidMaterial->setDiffuse(color);
    cuboidMaterial->setAmbient(color);

    //Cuboid
    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(root);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(cuboidMaterial);
    cuboidEntity->addComponent(cuboidTransform);

    cuboidEntity->setObjectName("cube");

    auto *picker = new Qt3DRender::QObjectPicker();
    picker->setHoverEnabled(true);
    picker->setEnabled(true);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, [=](){
        cuboidMaterial->setAmbient(QColor("yellow"));
        emit entityClicked(cuboidEntity, color);
    });
    cuboidEntity->addComponent(picker);

    return cuboidEntity;
}

Qt3DCore::QEntity* Drawing3d::drawSphere(QVector3D pos, double radius, QColor color, Qt3DCore::QEntity *root) {
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRadius(radius);

    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    // material->setDiffuse(color);
    material->setAmbient(color);

    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(pos);

    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(root);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(material);
    sphereEntity->addComponent(transform);

    sphereEntity->setObjectName("sphere");

    auto *picker = new Qt3DRender::QObjectPicker();
    picker->setHoverEnabled(true);
    picker->setEnabled(true);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, [=](){
        material->setAmbient(QColor("yellow"));
        emit entityClicked(sphereEntity, color);
    });
    sphereEntity->addComponent(picker);

    return sphereEntity;
}

void Drawing3d::drawSceneLoader(QVector3D pos, Qt3DCore::QEntity *root) {
    // Создаем загрузчик сцены
    Qt3DCore::QEntity *sceneLoaderEntity = new Qt3DCore::QEntity(root);
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setTranslation(pos);
    sceneLoaderEntity->addComponent(transform);

    Qt3DExtras::QDiffuseSpecularMaterial *objMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    sceneLoaderEntity->addComponent(objMaterial);

    Qt3DRender::QSceneLoader *sceneLoader = new Qt3DRender::QSceneLoader(sceneLoaderEntity);

    sceneLoader->setSource(QUrl::fromLocalFile("/home/user061/projects/3dViewer_on_3dCore/res/drum.obj"));

    QObject::connect(sceneLoader, &Qt3DRender::QSceneLoader::statusChanged, this,
                     [=](Qt3DRender::QSceneLoader::Status s) {
                         if (s == Qt3DRender::QSceneLoader::Ready) {
                             // Создаем одну общую трансформацию для всей модели
                             // QVector<Qt3DCore::QEntity *> entities = sceneLoader->entities();
                             // Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
                             // transform->setScale(0.1f);
                             // if(!entities.empty()) {
                             //     entities[0]->addComponent(transform);
                             // }

                             for(auto name : sceneLoader->entityNames()) {
                                 qDebug() << name;
                                 // Qt3DCore::QEntity *loadedEntity = sceneLoader->entity(name);
                                 // if (loadedEntity) {
                                 //     Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
                                 //     transform->setScale(0.1f);

                                 //     loadedEntity->addComponent(transform);
                                 // }
                             }
                         }
                     });

    sceneLoaderEntity->addComponent(sceneLoader);

    // root->addComponent(sceneLoaderEntity);
}

Qt3DCore::QEntity* Drawing3d::drawTexture(QVector3D pos, Qt3DCore::QEntity *root) {


    // Qt3DExtras::QSphereMesh *objMesh = new Qt3DExtras::QSphereMesh();
    // objMesh->setRadius(1.0f); // Радиус сферы
    // objMesh->setRings(20);    // Количество колец
    // objMesh->setSlices(20);   // Количество сегментов

    Qt3DRender::QMesh *objMesh = new Qt3DRender::QMesh();
    const QUrl url = QUrl::fromLocalFile("/home/user061/projects/3dViewer_on_3dCore/res/drum.obj");
    objMesh->setSource(url);

    // Создаем материал
    // Qt3DExtras::QDiffuseSpecularMaterial *objMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    // objMaterial->setDiffuse(QColor(Qt::white)); // Устанавливаем белый цвет

    Qt3DExtras::QDiffuseSpecularMaterial *objMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();

    Qt3DRender::QTextureLoader *textureLoader1 = new Qt3DRender::QTextureLoader();
    textureLoader1->setSource(QUrl::fromLocalFile("/home/user061/projects/3dViewer_on_3dCore/res/drum_DefaultMaterial_BaseColor.png"));
    textureLoader1->setWrapMode(Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::Repeat));
    objMaterial->setDiffuse(QVariant::fromValue(textureLoader1));
    objMaterial->setAmbient(QColor(0, 0, 100));

    Qt3DCore::QTransform *objTransform = new Qt3DCore::QTransform();
    objTransform->setScale(1.0f);
    objTransform->setTranslation(pos);

    Qt3DCore::QEntity *objEntity = new Qt3DCore::QEntity(root);

    objEntity->addComponent(objMesh);
    objEntity->addComponent(objMaterial);
    objEntity->addComponent(objTransform);

    objEntity->setObjectName("texture cube");

    return objEntity;
}

Qt3DCore::QEntity* Drawing3d::drawObj(QString filePath, QVector3D pos, QColor color, double scale, Qt3DCore::QEntity *root) {
    // Загружаем меш из файла .obj
    Qt3DRender::QMesh *objMesh = new Qt3DRender::QMesh();
    const QUrl url = QUrl(filePath);

    objMesh->setSource(url);

    // Создаем материал для объекта
    Qt3DExtras::QPhongMaterial *objMaterial = new Qt3DExtras::QPhongMaterial();
    objMaterial->setDiffuse(color);
    objMaterial->setAmbient(color);


    // Qt3DExtras::QDiffuseSpecularMaterial *objMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    // Qt3DRender::QTextureLoader *textureLoader1 = new Qt3DRender::QTextureLoader();
    // textureLoader1->setSource(QUrl("qrc:/textures/aluminium_random_brushed/aluminium_random_brushed_metallic.png"));
    // objMaterial->setDiffuse(QVariant::fromValue(textureLoader1));
    // Qt3DRender::QTextureLoader *textureLoader2 = new Qt3DRender::QTextureLoader();
    // textureLoader2->setSource(QUrl("qrc:/textures/aluminium_random_brushed/aluminium_random_brushed_basecolor.png"));
    // objMaterial->setNormal(QVariant::fromValue(textureLoader2));


    // Создаем трансформацию для объекта
    Qt3DCore::QTransform *objTransform = new Qt3DCore::QTransform();
    objTransform->setScale(scale);
    objTransform->setTranslation(pos);

    Qt3DCore::QEntity *objEntity = new Qt3DCore::QEntity(root);

    objEntity->addComponent(objMesh);
    objEntity->addComponent(objMaterial);
    objEntity->addComponent(objTransform);

    objEntity->setObjectName("obj");

    auto *picker = new Qt3DRender::QObjectPicker();
    picker->setHoverEnabled(true);
    picker->setEnabled(true);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, [=](){
        // objMaterial->setAmbient(QColor("yellow"));
        emit entityClicked(objEntity, color);
    });
    objEntity->addComponent(picker);

    return objEntity;
}

Qt3DCore::QEntity* Drawing3d::drawLine(double x1, double y1, double z1,
                                       double x2, double y2, double z2,
                                       QColor color, QString name,
                                       Qt3DCore::QEntity *root)
{
    QVector3D start(x1, y1, z1);
    QVector3D end(x2, y2, z2);

    auto *geometry = new Qt3DRender::QGeometry(root);

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
    auto *line = new Qt3DRender::QGeometryRenderer(root);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(root);
    material->setAmbient(color);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(root);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    lineEntity->setObjectName(name);

    if(name == "line") {
        auto *picker = new Qt3DRender::QObjectPicker();
        picker->setHoverEnabled(true);
        picker->setEnabled(true);
        QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, [=](){
            material->setAmbient(QColor("yellow"));
            emit entityClicked(lineEntity, color);
        });
        lineEntity->addComponent(picker);
    }

    return lineEntity;
}

QVector<Qt3DCore::QEntity *> Drawing3d::createGrid(double minX, double minY, double minZ,
                                                   double maxX, double maxY, double maxZ,
                                                   uint32_t segmentCountX,
                                                   uint32_t segmentCountY,
                                                   uint32_t segmentCountZ,
                                                   Qt3DCore::QEntity *root)
{
    QVector<Qt3DCore::QEntity *> lines;

    for (float i = minX; i <= maxX; i += segmentCountX) {
        lines.append(this->drawLine(i, minY, 0,
                                    i, maxY, 0,
                                    Qt::red, "X",
                                    root)
                     );

        lines.append(this->drawLine(i, 0, minZ,
                                    i, 0, maxZ,
                                    Qt::green, "Z",
                                    root)
                     );
    }


    for (float i = minY; i <= maxY; i += segmentCountY) {
        lines.append(this->drawLine(minX, i, 0,
                                    maxX, i, 0,
                                    Qt::red, "X",
                                    root)
                     );

        lines.append(this->drawLine(0, i, minZ,
                                    0, i, maxZ,
                                    Qt::blue, "Y",
                                    root)
                     );
    }

    for (float i = minZ; i <= maxZ; i += segmentCountZ) {
        lines.append(this->drawLine(minX, 0, i,
                                    maxX, 0, i,
                                    Qt::green, "Z",
                                    root)
                     );

        lines.append(this->drawLine(0, minY, i,
                                    0, maxY, i,
                                    Qt::blue, "Y",
                                    root)
                     );
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
