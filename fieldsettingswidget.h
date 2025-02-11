#ifndef FIELDSETTINGSWIDGET_H
#define FIELDSETTINGSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSettings>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "settings.h"

namespace Ui {
class FieldSettingsWidget;
}

class FieldSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FieldSettingsWidget(QWidget *parent = nullptr);
    ~FieldSettingsWidget();

    // double minX() const;
    // double minY() const;
    // double minZ() const;
    // double maxX() const;
    // double maxY() const;
    // double maxZ() const;
    // uint32_t segmentCountX() const;
    // uint32_t segmentCountY() const;
    // uint32_t segmentCountZ() const;
    // bool isGridHide() const;
    // bool isStarSkyHide() const;
    // uint32_t starsNumber() const;

    GridSettings settings() const;

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::FieldSettingsWidget *ui;
    void saveSettings();
    void loadSettings();
    void updateParameters();

    GridSettings m_settings;

signals:
    void closeSignal();
    void updateFieldSignal();
};

#endif // FIELDSETTINGSWIDGET_H
