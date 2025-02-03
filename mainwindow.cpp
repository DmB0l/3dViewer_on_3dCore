#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fieldSettingsWidget(new FieldSettingsWidget)
    , m_cameraSettingsWidget(new CameraSettingsWidget)
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

    // UPDATE SETTINGS CONNECTIONS
    connect(m_fieldSettingsWidget, &FieldSettingsWidget::updateFieldSignal, this, &MainWindow::updateField);
    connect(m_cameraSettingsWidget, &CameraSettingsWidget::updateCameraSettings, this, &MainWindow::updateCameraSettings);
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
    for(int i = 0; i < 20; i++) {
        m_viewElements.append(m_drawing->drawCube(QVector3D(i + 0.5, 0.5, i + 0.5), 1.0f, QColor(QRgb(0x665423)), m_rootEntity));
    }




    //HEART
    // m_drawing->createHeart(m_rootEntity, QVector3D(20, 20, 20));


    //OBJ
    m_viewElements.append(m_drawing->drawObj("file:///home/user061/projects/3dViewer_on_3dCore/res/cow-nonormals.obj", QVector3D(50,3.5,50), QColor("blue"), m_rootEntity));
    m_viewElements.append(m_drawing->drawObj("qrc:/res/drone_model.obj", QVector3D(20,20,20), QColor("white"), m_rootEntity));
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

void MainWindow::closeEvent(QCloseEvent *event) {
    if(m_fieldSettingsWidget) {
        m_fieldSettingsWidget->close();
    }

    if(m_cameraSettingsWidget) {
        m_cameraSettingsWidget->close();
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
