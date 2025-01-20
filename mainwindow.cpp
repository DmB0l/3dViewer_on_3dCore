#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_fieldSettingsWidget(new FieldSettingsWidget)
    , m_api(new API(this))
    , m_drawing(new Drawing3d(this))
{
    ui->setupUi(this);

    m_rootEntity = new Qt3DCore::QEntity();

    Custom3dWindow *view = new Custom3dWindow(m_rootEntity);

    QWidget *container = QWidget::createWindowContainer(view);
    ui->centralwidget->layout()->setMargin(0);
    QGridLayout *layout3d = new QGridLayout(ui->W_3dWidget);
    layout3d->addWidget(container);
    layout3d->setMargin(0);

    connect(ui->PB_grid, &QPushButton::clicked, this, &MainWindow::showGridSettings);
    connect(m_fieldSettingsWidget, &FieldSettingsWidget::updateFieldSignal, this, &MainWindow::updateField);

    m_drawing->createStarrySky(m_rootEntity, m_starsNumber,
                               m_fieldSettingsWidget->minX(),
                               m_fieldSettingsWidget->minY(),
                               m_fieldSettingsWidget->minZ(),
                               m_fieldSettingsWidget->maxX(),
                               m_fieldSettingsWidget->maxY(),
                               m_fieldSettingsWidget->maxZ());

    m_drawing->createHeart(m_rootEntity);

    if(!m_fieldSettingsWidget->isGridHide()) {
        m_grid = m_drawing->createGrid(m_fieldSettingsWidget->minX(),
                                       m_fieldSettingsWidget->minY(),
                                       m_fieldSettingsWidget->minZ(),
                                       m_fieldSettingsWidget->maxX(),
                                       m_fieldSettingsWidget->maxY(),
                                       m_fieldSettingsWidget->maxZ(),
                                       m_fieldSettingsWidget->segmentCountX(),
                                       m_fieldSettingsWidget->segmentCountY(),
                                       m_fieldSettingsWidget->segmentCountZ(),
                                       m_rootEntity);
    }

    for(int i = 0; i < 20; i++) {
        Qt3DCore::QEntity *entity = m_drawing->drawSimpleCube(QVector3D(i + 0.5, 0.5, i + 0.5), 1.0f, QColor(QRgb(0x665423)), m_rootEntity);
        m_viewElements.append(entity);
    }

    connect(m_api, &API::addLine, this, [=](double x1, double y1, double z1,
                                            double x2, double y2, double z2,
                                            QColor color){
        m_drawing->drawLine(x1, y1, z1, x2, y2, z2, color, m_rootEntity);
    });

    connect(m_api, &API::addCube, this, [=](double x, double y, double z,
                                            double size,
                                            QColor color){
        m_drawing->drawSimpleCube(QVector3D(x, y, z), size, color, m_rootEntity);
    });

    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_fieldSettingsWidget) {
        delete m_fieldSettingsWidget;
    }
}

void MainWindow::updateField() {
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

    // this->setFocus();

    if(!m_fieldSettingsWidget->isGridHide()) {
        m_grid = m_drawing->createGrid(m_fieldSettingsWidget->minX(),
                                       m_fieldSettingsWidget->minY(),
                                       m_fieldSettingsWidget->minZ(),
                                       m_fieldSettingsWidget->maxX(),
                                       m_fieldSettingsWidget->maxY(),
                                       m_fieldSettingsWidget->maxZ(),
                                       m_fieldSettingsWidget->segmentCountX(),
                                       m_fieldSettingsWidget->segmentCountY(),
                                       m_fieldSettingsWidget->segmentCountZ(),
                                       m_rootEntity);
    }

    m_drawing->createStarrySky(m_rootEntity, m_starsNumber,
                                           m_fieldSettingsWidget->minX(),
                                           m_fieldSettingsWidget->minY(),
                                           m_fieldSettingsWidget->minZ(),
                                           m_fieldSettingsWidget->maxX(),
                                           m_fieldSettingsWidget->maxY(),
                                           m_fieldSettingsWidget->maxZ());
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



void MainWindow::closeEvent(QCloseEvent *event) {
    if(m_fieldSettingsWidget) {
        m_fieldSettingsWidget->close();
    }
}
