#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fieldSettingsWidget(new FieldSettingsWidget)
    , m_cameraSettingsWidget(new CameraSettingsWidget)
    , m_colorSettingsWidget(new ColorSettingsWidget)
    , m_api(new API(this))
    , m_drawing(new Drawing3d(this))
{
    ui->setupUi(this);

    m_qmlView = new View3d(ui->quickWidget, this);

    // SETTINGS
    m_gridSettings = m_fieldSettingsWidget->settings();
    m_cameraSettings = m_cameraSettingsWidget->cameraSettings();

    m_qmlView->setCameraSettings(m_cameraSettings);
    m_qmlView->setPortText(QString::number(m_api->listenPort()));
    m_qmlView->setColorSettings(m_colorSettingsWidget->backgroundColor());

    // 3D
    m_rootEntity = m_qmlView->rootEntity();

    // CENTRAL WIDGET
    ui->centralwidget->layout()->setMargin(0);
    ui->centralwidget->layout()->setSpacing(0);
    ui->centralwidget->layout()->setContentsMargins(0, 0, 0, 0);

    // ENTITYES CLICKED CONNECTION
    connect(m_drawing, &Drawing3d::entityClicked, this, &MainWindow::onEntityClicked);

    // PUSH BUTTONS CONNECTIONS
    connect(ui->PB_grid, &QPushButton::clicked, this, &MainWindow::showGridSettings);
    connect(ui->PB_cameraSettings, &QPushButton::clicked, this, &MainWindow::showCameraSettings);
    connect(ui->PB_colorSettings, &QPushButton::clicked, this, &MainWindow::showColorSettings);

    // UPDATE SETTINGS CONNECTIONS
    connect(m_fieldSettingsWidget, &FieldSettingsWidget::updateFieldSignal, this, &MainWindow::updateField);
    connect(m_cameraSettingsWidget, &CameraSettingsWidget::updateCameraSettings, this, &MainWindow::updateCameraSettings);
    connect(m_colorSettingsWidget, &ColorSettingsWidget::updateColorSettings, this, &MainWindow::updateColorSettings);
    connect(m_qmlView, &View3d::updatePortSettings, this, [=](QString port){
        m_api->setListenPort(port.toInt());
    });

    //API CONNECTIONS
    connect(m_api, &API::addLine, this, [=](double x1, double y1, double z1,
                                            double x2, double y2, double z2,
                                            QColor color){
        m_viewElements.append(m_drawing->drawLine(x1, y1, z1, x2, y2, z2, color, "line", m_rootEntity));
    });

    connect(m_api, &API::addCube, this, [=](double x, double y, double z,
                                            double size,
                                            QColor color){
        m_viewElements.append(m_drawing->drawCube(QVector3D(x, y, z), size, color, m_rootEntity));
    });

    connect(m_api, &API::addSphere, this, [=](double x, double y, double z,
                                              double radius,
                                              QColor color){
        m_viewElements.append(m_drawing->drawSphere(QVector3D(x, y, z), radius, color, m_rootEntity));
    });

    connect(m_api, &API::addPlane, this, [=](double x, double y, double z,
                                             double width, double height,
                                             QColor color){
        m_viewElements.append(m_drawing->drawPlane(QVector3D(x, y, z), width, height, color, m_rootEntity));
    });

    connect(m_api, &API::clearAll, this, [=](){
        if(m_viewElements.size() != 0) {
            for(int i = 0; i < m_viewElements.size(); i++) {
                m_viewElements.at(i)->setEnabled(false);
                m_viewElements.at(i)->deleteLater();
            }
            m_viewElements.clear();
        }
    });



    //STARS SKY
    if(!m_gridSettings.isStarSkyHide) {
        m_starSky = m_drawing->createStarrySky(m_rootEntity,
                                               m_gridSettings.starsNumber,
                                               m_gridSettings.minX,
                                               m_gridSettings.minY,
                                               m_gridSettings.minZ,
                                               m_gridSettings.maxX,
                                               m_gridSettings.maxY,
                                               m_gridSettings.maxZ);
    }


    //GRID
    if(!m_gridSettings.isGridHide) {
        m_grid = m_drawing->createGrid(m_gridSettings.minX,
                                       m_gridSettings.minY,
                                       m_gridSettings.minZ,
                                       m_gridSettings.maxX,
                                       m_gridSettings.maxY,
                                       m_gridSettings.maxZ,
                                       m_gridSettings.segmentCountX,
                                       m_gridSettings.segmentCountY,
                                       m_gridSettings.segmentCountZ,
                                       m_rootEntity);
    }

    //CUBES
    // for(int i = 0; i < 20; i++) {
    //     m_viewElements.append(m_drawing->drawCube(QVector3D(i + 0.5, 0.5, i + 0.5), 1.0f, QColor(QRgb(0x665423)), m_rootEntity));
    // }




    //HEART
    // m_drawing->createHeart(m_rootEntity, QVector3D(20, 20, 20));


    //OBJ
    Qt3DCore::QEntity *caw = m_drawing->drawObj("file:///home/user061/projects/3dViewer_on_3dCore/res/cow-nonormals.obj",
                                                QVector3D(50,3.5,50),
                                                QColor("blue"),
                                                1.0,
                                                m_rootEntity);

    Qt3DCore::QEntity *drone = m_drawing->drawObj("qrc:/res/drone_model.obj",
                                                  QVector3D(20,20,20),
                                                  QColor("pink"),
                                                  0.2,
                                                  m_rootEntity);

    // m_viewElements.append(caw);
    // m_viewElements.append(drone);


    //ANIMATION DRONE
    Qt3DCore::QTransform *transformDrone = drone->componentsOfType<Qt3DCore::QTransform>().at(0);

    // Анимация вращения
    QPropertyAnimation *rotationAnimationDrone = new QPropertyAnimation(transformDrone);
    rotationAnimationDrone->setTargetObject(transformDrone);
    rotationAnimationDrone->setPropertyName("rotationX");
    rotationAnimationDrone->setStartValue(QVariant::fromValue(0));
    rotationAnimationDrone->setEndValue(QVariant::fromValue(360));
    rotationAnimationDrone->setDuration(5000); // 5 секунд
    rotationAnimationDrone->setLoopCount(-1); // Бесконечная анимация
    rotationAnimationDrone->start();

    // Анимация передвижения
    QPropertyAnimation *moveAnimationDrone = new QPropertyAnimation(transformDrone);
    moveAnimationDrone->setTargetObject(transformDrone);
    moveAnimationDrone->setPropertyName("translation");
    moveAnimationDrone->setStartValue(QVector3D(0, 0, 0));
    moveAnimationDrone->setEndValue(QVector3D(800, 800, 800));
    moveAnimationDrone->setDuration(300000); // 5 секунд
    // moveAnimation->setLoopCount(-1); // Бесконечная анимация
    moveAnimationDrone->start();


    //ANIMATION CAW
    Qt3DCore::QTransform *transformCaw = caw->componentsOfType<Qt3DCore::QTransform>().at(0);

    // Анимация вращения
    QPropertyAnimation *rotationAnimationCaw = new QPropertyAnimation(transformCaw);
    rotationAnimationCaw->setTargetObject(transformCaw);
    rotationAnimationCaw->setPropertyName("rotationY");
    rotationAnimationCaw->setStartValue(QVariant::fromValue(0));
    rotationAnimationCaw->setEndValue(QVariant::fromValue(360));
    rotationAnimationCaw->setDuration(10000); // 5 секунд
    rotationAnimationCaw->setLoopCount(-1); // Бесконечная анимация
    rotationAnimationCaw->start();

    // Анимация передвижения
    QPropertyAnimation *moveAnimationCaw = new QPropertyAnimation(transformCaw);
    moveAnimationCaw->setTargetObject(transformCaw);
    moveAnimationCaw->setPropertyName("translation");
    moveAnimationCaw->setStartValue(QVector3D(50, 3.5, 50));
    moveAnimationCaw->setEndValue(QVector3D(50, 50, 50));
    moveAnimationCaw->setDuration(10000); // 5 секунд
    // moveAnimation->setLoopCount(-1); // Бесконечная анимация
    moveAnimationCaw->start();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        QVector3D pos1 = transformDrone->translation();
        QVector3D pos2 = transformCaw->translation();

        if ((pos1 - pos2).length() < 3.0f) { // Расстояние меньше порогового значения
            ExplosionEffect *explosion = new ExplosionEffect(m_rootEntity);
            explosion->setPosition((pos1 + pos2) / 2.0f);

            timer->stop();

            drone->setEnabled(false);
            caw->setEnabled(false);

            drone->deleteLater();
            caw->deleteLater();

            // m_viewElements.removeOne(caw);
            // m_viewElements.removeOne(drone);
        }
    });
    timer->start(16);



    //КОРОВИЙ АД
    // QTimer *timerHell = new QTimer(this);
    // connect(timerHell, &QTimer::timeout, this, [=]() {
    //     int coordX = m_gen->bounded(0, 100);
    //     int coordY = m_gen->bounded(0, 100);
    //     int coordZ = m_gen->bounded(0, 100);

    //     int r = m_gen->bounded(0, 255);
    //     int g = m_gen->bounded(0, 255);
    //     int b = m_gen->bounded(0, 255);

    //     double size = m_gen->bounded(0, 3);

    //     Qt3DCore::QEntity *cawHell = m_drawing->drawObj("file:///home/user061/projects/3dViewer_on_3dCore/res/cow-nonormals.obj",
    //                                                     QVector3D(coordX, coordY, coordZ),
    //                                                     QColor(r, g, b),
    //                                                     size,
    //                                                     m_rootEntity);
    //     m_viewElements.append(cawHell);

    //     Qt3DCore::QTransform *transformCaw = cawHell->componentsOfType<Qt3DCore::QTransform>().at(0);

    //     int endX = m_gen->bounded(-50, 50);
    //     int endY = m_gen->bounded(-50, 50);
    //     int endZ = m_gen->bounded(-50, 50);

    //     int timeAnimation = m_gen->bounded(1000, 30000);

    //     // Анимация передвижения
    //     QPropertyAnimation *moveAnimationCaw = new QPropertyAnimation(transformCaw);
    //     moveAnimationCaw->setTargetObject(transformCaw);
    //     moveAnimationCaw->setPropertyName("translation");
    //     moveAnimationCaw->setStartValue(QVector3D(coordX, coordY, coordZ));
    //     moveAnimationCaw->setEndValue(QVector3D(endX, endY, endZ));
    //     moveAnimationCaw->setDuration(timeAnimation); // 5 секунд
    //     moveAnimationCaw->setLoopCount(-1); // Бесконечная анимация
    //     moveAnimationCaw->start();

    //     QTimer *timerHell2 = new QTimer(this);
    //     connect(timerHell2, &QTimer::timeout, this, [=]() {
    //         // Создаем временный список для безопасного удаления
    //         QList<Qt3DCore::QEntity*> toRemove;

    //         for (int i = 0; i < m_viewElements.size(); ++i) {
    //             if (m_viewElements.at(i) != cawHell) {
    //                 if(cawHell != nullptr && transformCaw != nullptr && m_viewElements.at(i) != nullptr) {
    //                     QVector3D pos1 = m_viewElements.at(i)->componentsOfType<Qt3DCore::QTransform>().at(0)->translation();
    //                     QVector3D pos2 = transformCaw->translation();

    //                     if ((pos1 - pos2).length() < 3.0f) { // Расстояние меньше порогового значения
    //                         ExplosionEffect *explosion = new ExplosionEffect(m_rootEntity);
    //                         explosion->setPosition((pos1 + pos2) / 2.0f);

    //                         if(transformCaw != nullptr && m_viewElements.at(i) != nullptr) {
    //                             // Отключаем объекты
    //                             m_viewElements.at(i)->setEnabled(false);
    //                             cawHell->setEnabled(false);

    //                             // Добавляем объекты в список для удаления
    //                             toRemove.append(m_viewElements.at(i));
    //                             toRemove.append(cawHell);
    //                             // transformCaw = nullptr;
    //                         }

    //                         break;
    //                     }
    //                 }
    //             }
    //         }

    //         // Удаляем объекты из сцены и списка
    //         for (Qt3DCore::QEntity *entity : toRemove) {
    //             if(entity != nullptr) {
    //                 m_viewElements.removeOne(entity);
    //                 entity->deleteLater();
    //                 entity = nullptr;
    //             }
    //         }

    //         // Если объекты удалены, останавливаем таймер
    //         if (!toRemove.isEmpty()) {
    //             timerHell2->stop();
    //             timerHell2->deleteLater();
    //         }
    //     });
    //     timerHell2->start(16);
    // });
    // timerHell->start(100);

    // QTimer *timerHell2 = new QTimer(this);
    // connect(timerHell2, &QTimer::timeout, this, [=]() {
    //     // QList<Qt3DCore::QEntity *> tmpList = m_viewElements;

    //     QList<Qt3DCore::QEntity *>::iterator it1 = m_viewElements.begin();;

    //     while (it1 != m_viewElements.end()) {
    //         QList<Qt3DCore::QEntity *>::iterator it2 = m_viewElements.begin();

    //         bool flag = true;
    //         while (it2 != m_viewElements.end()) {
    //             if (it1 == it2) {
    //                 ++it2;
    //                 continue;
    //             }

    //             QVector3D pos1 = (*it1)->componentsOfType<Qt3DCore::QTransform>().at(0)->translation();
    //             QVector3D pos2 = (*it2)->componentsOfType<Qt3DCore::QTransform>().at(0)->translation();

    //             // qDebug() << (pos1 - pos2).length();

    //             if ((pos1 - pos2).length() < 3.0f) {
    //                 ExplosionEffect *explosion = new ExplosionEffect(m_rootEntity);
    //                 explosion->setPosition((pos1 + pos2) / 2.0f);

    //                 // Сохраняем указатели на объекты
    //                 Qt3DCore::QEntity *entity1 = *it1;
    //                 Qt3DCore::QEntity *entity2 = *it2;

    //                 // QList<Qt3DCore::QEntity *>::iterator tmpIt = it1;

    //                 m_viewElements.removeOne((*it2));
    //                 m_viewElements.removeOne((*it1));
    //                 // it2 = tmpList.erase(it2);
    //                 // it1 = tmpList.erase(tmpIt);

    //                 entity1->setEnabled(false);
    //                 // entity1->deleteLater();
    //                 entity2->setEnabled(false);
    //                 // entity2->deleteLater();

    //                 it1 = m_viewElements.begin();
    //                 flag = false;
    //                 break;
    //             }
    //             else {
    //                 ++it2;
    //             }
    //         }
    //         if(flag)
    //             ++it1;
    //     }
    // });
    // timerHell2->start(16);

}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_fieldSettingsWidget) {
        delete m_fieldSettingsWidget;
    }

    if(m_fieldSettingsWidget) {
        delete m_cameraSettingsWidget;
    }
}

void MainWindow::updateField() {
    m_gridSettings = m_fieldSettingsWidget->settings();

    for(int i = 0; i < m_grid.size(); i++) {
        m_grid.at(i)->setEnabled(false);
        m_grid.at(i)->deleteLater();
    }
    m_grid.clear();

    for(int i = 0; i < m_starSky.size(); i++) {
        m_starSky.at(i)->setEnabled(false);
        m_starSky.at(i)->deleteLater();
    }
    m_starSky.clear();

    if(!m_gridSettings.isGridHide) {
        m_grid = m_drawing->createGrid(m_gridSettings.minX,
                                       m_gridSettings.minY,
                                       m_gridSettings.minZ,
                                       m_gridSettings.maxX,
                                       m_gridSettings.maxY,
                                       m_gridSettings.maxZ,
                                       m_gridSettings.segmentCountX,
                                       m_gridSettings.segmentCountY,
                                       m_gridSettings.segmentCountZ,
                                       m_rootEntity);
    }

    if(!m_gridSettings.isStarSkyHide) {
        m_starSky = m_drawing->createStarrySky(m_rootEntity,
                                               m_gridSettings.starsNumber,
                                               m_gridSettings.minX,
                                               m_gridSettings.minY,
                                               m_gridSettings.minZ,
                                               m_gridSettings.maxX,
                                               m_gridSettings.maxY,
                                               m_gridSettings.maxZ);
    }
}

void MainWindow::updateCameraSettings() {
    m_cameraSettings = m_cameraSettingsWidget->cameraSettings();
    m_qmlView->setCameraSettings(m_cameraSettings);
}

void MainWindow::updateColorSettings() {
    m_qmlView->setColorSettings(m_colorSettingsWidget->backgroundColor());
}

void MainWindow::onEntityClicked(Qt3DCore::QEntity *entity, QColor color){
    if(m_selectedEntity != nullptr) {
        m_selectedEntity->componentsOfType<Qt3DExtras::QPhongMaterial>().at(0)->setAmbient(m_prevColorSelectedEntity);
    }
    if(m_selectedEntity != entity) {
        m_selectedEntity = entity;
        m_prevColorSelectedEntity = color;
        m_qmlView->createSelectedEntityText(entity);
    }
    else {
        m_selectedEntity = nullptr;
        m_qmlView->clearSelectedEntityText();
    }
}

void MainWindow::showGridSettings() {
    if(m_fieldSettingsWidget) {
        m_fieldSettingsWidget->setFocus();
        // this->setEnabled(false);

        connect(m_fieldSettingsWidget, &FieldSettingsWidget::closeSignal, this, [=](){
            this->setEnabled(true);
        });

        m_fieldSettingsWidget->show();
    }
}

void MainWindow::showCameraSettings() {
    if(m_cameraSettingsWidget) {
        m_cameraSettingsWidget->setFocus();
        // this->setEnabled(false);

        connect(m_cameraSettingsWidget, &CameraSettingsWidget::closeSignal, this, [=](){
            this->setEnabled(true);
        });

        m_cameraSettingsWidget->show();
    }
}

void MainWindow::showColorSettings() {
    if(m_colorSettingsWidget) {
        // m_colorSettingsWidget->setFocus();
        // this->setEnabled(false);

        connect(m_colorSettingsWidget, &ColorSettingsWidget::closeSignal, this, [=](){
            this->setEnabled(true);
        });

        m_colorSettingsWidget->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(m_fieldSettingsWidget) {
        m_fieldSettingsWidget->close();
    }

    if(m_cameraSettingsWidget) {
        m_cameraSettingsWidget->close();
    }

    if(m_colorSettingsWidget) {
        m_colorSettingsWidget->close();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete) {
        if(m_selectedEntity != nullptr) {
            m_selectedEntity->setEnabled(false);
            m_selectedEntity->deleteLater();
            m_selectedEntity = nullptr;
            m_qmlView->clearSelectedEntityText();
        }
    }
}
