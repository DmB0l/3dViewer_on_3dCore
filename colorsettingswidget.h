#ifndef COLORSETTINGSWIDGET_H
#define COLORSETTINGSWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QPalette>
#include <QSettings>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Ui {
class ColorSettingsWidget;
}

class ColorSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSettingsWidget(QWidget *parent = nullptr);
    ~ColorSettingsWidget();

    QColor backgroundColor() const;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void pickColor(bool checked);

signals:
    void closeSignal();
    void updateColorSettings();

private:
    void loadSettings();
    void saveSettings();

    Ui::ColorSettingsWidget *ui;
    QColor m_backgroundColor = QColor("black");
    // QSettings m_settings;



};

#endif // COLORSETTINGSWIDGET_H
