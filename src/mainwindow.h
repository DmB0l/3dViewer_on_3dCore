#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
#include <Qt3DExtras/QText2DEntity>
#include <Qt3DAnimation>

#include <QSharedPointer>
#include <QRandomGenerator>
#include <QLabel>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "drawing3d.h"
#include "fieldsettingswidget.h"
#include "camerasettingswidget.h"
#include "colorsettingswidget.h"
#include "api.h"
#include "view3d.h"
#include "explosioneffect.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onEntityClicked(Qt3DCore::QEntity *entity, QColor color);
    void updateField();
    void updateCameraSettings();
    void updateColorSettings();
    void showGridSettings();
    void showCameraSettings();
    void showColorSettings();
    void addCowHell();

signals:
    void addExplosion(QVector3D pos);

private:
    Ui::MainWindow *ui;
    Qt3DCore::QEntity *m_rootEntity;
    API *m_api;
    Drawing3d *m_drawing;
    View3d *m_qmlView;

    Qt3DCore::QEntity *m_selectedEntity = nullptr;
    QColor m_prevColorSelectedEntity;

    GridSettings m_gridSettings;
    CameraSettings m_cameraSettings;

    QList<Qt3DCore::QEntity *> m_viewElements;
    QVector<Qt3DCore::QEntity *> m_grid;
    QVector<Qt3DCore::QEntity *> m_starSky;

    FieldSettingsWidget *m_fieldSettingsWidget = nullptr;
    CameraSettingsWidget *m_cameraSettingsWidget = nullptr;
    ColorSettingsWidget *m_colorSettingsWidget = nullptr;

    // Надписи для infoWidget
    QWidget* m_infoWidget = nullptr; // Widget для отображения информации о камере
    QGridLayout *m_gridLayoutInfoWidget = nullptr; // Layout
    QLabel *m_labelCameraNumber = nullptr;
    QLabel *m_labelFps = nullptr;
    QLabel *m_labelFrameType = nullptr;
    QLabel *m_labelGetFrames = nullptr;
    QLabel *m_labelLostFrames = nullptr;
    QLabel *m_labelResolution = nullptr;
    QLabel *m_labelSourceType = nullptr;
    QLabel *m_labelTime = nullptr;

    QRandomGenerator *m_gen = QRandomGenerator::global();

    QMutex *m_mutex = nullptr;
    QThread *m_thread = nullptr;
    bool m_isThreadWork = true;



};

#endif // MAINWINDOW_H
