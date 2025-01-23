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

    // SETTINGS
    m_gridSettings = m_fieldSettingsWidget->settings();
    m_cameraSettings = m_cameraSettingsWidget->cameraSettings();

    // 3D
    m_rootEntity = new Qt3DCore::QEntity();
    m_view = new Custom3dWindow(m_rootEntity);
    m_view->setCameraSettings(m_cameraSettings);

    // m_customGraphicsView = new CustomGraphicsView(QWidget::createWindowContainer(m_view), this);
    // ui->L_view->addWidget(m_customGraphicsView);

    QWidget* container = QWidget::createWindowContainer(m_view);
    ui->W_mainWidget->layout()->addWidget(container);
    ui->W_mainWidget->layout()->setMargin(0);
    ui->W_mainWidget->layout()->setSpacing(0);
    ui->W_mainWidget->layout()->setContentsMargins(0, 0, 0, 0);


    // PUSH BUTTONS CONNECTIONS
    connect(ui->PB_grid, &QPushButton::clicked, this, &MainWindow::showGridSettings);
    connect(ui->PB_cameraSettings, &QPushButton::clicked, this, &MainWindow::showCameraSettings);

    // UPDATE SETTINGS CONNECTIONS
    connect(m_fieldSettingsWidget, &FieldSettingsWidget::updateFieldSignal, this, &MainWindow::updateField);
    connect(m_cameraSettingsWidget, &CameraSettingsWidget::updateCameraSettings, this, &MainWindow::updateCameraSettings);

    //API CONNECTIONS
    connect(m_api, &API::addLine, this, [=](double x1, double y1, double z1,
                                            double x2, double y2, double z2,
                                            QColor color){
        m_drawing->drawLine(x1, y1, z1, x2, y2, z2, color, m_rootEntity);
    });

    connect(m_api, &API::addCube, this, [=](double x, double y, double z,
                                            double size,
                                            QColor color){
        m_drawing->drawCube(QVector3D(x, y, z), size, color, m_rootEntity);
    });

    connect(m_api, &API::addSphere, this, [=](double x, double y, double z,
                                              double radius,
                                              QColor color){
        m_drawing->drawSphere(QVector3D(x, y, z), radius, color, m_rootEntity);
    });

    connect(m_api, &API::addPlane, this, [=](double x, double y, double z,
                                             double width, double height,
                                             QColor color){
        m_drawing->drawPlane(QVector3D(x, y, z), width, height, color, m_rootEntity);
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
        Qt3DCore::QEntity *entity = m_drawing->drawCube(QVector3D(i + 0.5, 0.5, i + 0.5), 1.0f, QColor(QRgb(0x665423)), m_rootEntity);
        m_viewElements.append(entity);
    }

    //HEART
    // m_drawing->createHeart(m_rootEntity, QVector3D(20, 20, 20));
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
    m_view->setCameraSettings(m_cameraSettings);
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
