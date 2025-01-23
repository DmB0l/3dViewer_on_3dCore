#ifndef CAMERASETTINGSWIDGET_H
#define CAMERASETTINGSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include "settings.h"

namespace Ui {
class CameraSettingsWidget;
}

class CameraSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CameraSettingsWidget(QWidget *parent = nullptr);
    ~CameraSettingsWidget();

    CameraSettings cameraSettings() const;

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void saveSettings();
    void loadSettings();
    void updateParameters();

    Ui::CameraSettingsWidget *ui;
    CameraSettings m_settings;

signals:
    void closeSignal();
    void updateCameraSettings();
};

#endif // CAMERASETTINGSWIDGET_H
