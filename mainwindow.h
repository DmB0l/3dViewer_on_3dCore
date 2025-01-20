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

#include <QSharedPointer>
#include <QRandomGenerator>

#include "custom3dwindow.h"
#include "lineentity.h"
#include "linemousehandler.h"
#include "drawing3d.h"
#include "fieldsettingswidget.h"
#include "api.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateField();
    void showGridSettings();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    Qt3DCore::QEntity *m_rootEntity;
    API *m_api;
    Drawing3d *m_drawing;

    QVector<Qt3DCore::QEntity *> m_viewElements;
    QVector<LineEntity *> m_grid;
    QVector<Qt3DCore::QEntity *> m_starSky;
    uint32_t m_starsNumber = 1000;

    FieldSettingsWidget *m_fieldSettingsWidget = nullptr;

    int m_counter = 0;
    void createStar(Qt3DCore::QEntity *parent, const QVector3D &position, float size);
    void createStarrySky(Qt3DCore::QEntity *rootEntity, int starCount);
};

#endif // MAINWINDOW_H
