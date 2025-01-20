#ifndef FIELDSETTINGSWIDGET_H
#define FIELDSETTINGSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSettings>

namespace Ui {
class FieldSettingsWidget;
}

class FieldSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FieldSettingsWidget(QWidget *parent = nullptr);
    ~FieldSettingsWidget();

    double minX() const;
    double minY() const;
    double minZ() const;
    double maxX() const;
    double maxY() const;
    double maxZ() const;
    uint32_t segmentCountX() const;
    uint32_t segmentCountY() const;
    uint32_t segmentCountZ() const;
    bool isGridHide() const;

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::FieldSettingsWidget *ui;
    void saveSettings();
    void loadSettings();
    void updateParameters();

    double m_minX = 0, m_minY = 0, m_minZ = 0;
    double m_maxX =  20, m_maxY =  20, m_maxZ =  20;
    uint32_t m_segmentCountX = 1;
    uint32_t m_segmentCountY = 1;
    uint32_t m_segmentCountZ = 1;

    bool m_isGridHide = false;

signals:
    void closeSignal();
    void updateFieldSignal();
};

#endif // FIELDSETTINGSWIDGET_H
