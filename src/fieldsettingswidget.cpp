#include "fieldsettingswidget.h"
#include "ui_fieldsettingswidget.h"

FieldSettingsWidget::FieldSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FieldSettingsWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Grid settings");

    loadSettings();

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        connect(okButton, &QPushButton::clicked, this, [=]() {
            saveSettings();
            emit updateFieldSignal();
            this->close();
        });
    }

    QPushButton *cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    if (cancelButton) {
        connect(cancelButton, &QPushButton::clicked, this, [=]() {
            this->close();
        });
    }

    QPushButton *applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton) {
        connect(applyButton, &QPushButton::clicked, this, [=]() {
            saveSettings();
            emit updateFieldSignal();
        });
    }
}

FieldSettingsWidget::~FieldSettingsWidget() {
    delete ui;
}

void FieldSettingsWidget::closeEvent(QCloseEvent *event) {
    emit closeSignal();
    QWidget::closeEvent(event); // Вызов базового класса
}

void FieldSettingsWidget::showEvent(QShowEvent *event) {
    loadSettings();
    updateParameters();
    QWidget::showEvent(event); // Вызов базового класса
}

void FieldSettingsWidget::saveSettings() {
    spdlog::info("Сохранение параметров сетки");

    m_settings.minX = ui->SB_minX->value();
    m_settings.minY = ui->SB_minY->value();
    m_settings.minZ = ui->SB_minZ->value();

    m_settings.maxX = ui->SB_maxX->value();
    m_settings.maxY = ui->SB_maxY->value();
    m_settings.maxZ = ui->SB_maxZ->value();

    m_settings.segmentCountX = ui->SB_stepX->value();
    m_settings.segmentCountY = ui->SB_stepY->value();
    m_settings.segmentCountZ = ui->SB_stepZ->value();

    m_settings.isGridHide = ui->CB_hideGrid->isChecked();
    m_settings.isStarSkyHide = ui->CB_hideStarSky->isChecked();

    m_settings.starsNumber = ui->SB_stars->value();

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("GRID PARAMETERS");
    settings.setValue("minX", m_settings.minX);
    settings.setValue("minY", m_settings.minY);
    settings.setValue("minZ", m_settings.minZ);

    settings.setValue("maxX", m_settings.maxX);
    settings.setValue("maxY", m_settings.maxY);
    settings.setValue("maxZ", m_settings.maxZ);
    settings.endGroup();

    settings.beginGroup("SEGMENT PARAMETERS");
    settings.setValue("stepX", m_settings.segmentCountX);
    settings.setValue("stepY", m_settings.segmentCountY);
    settings.setValue("stepZ", m_settings.segmentCountZ);
    settings.endGroup();

    settings.beginGroup("OPTIONS");
    settings.setValue("hideGrid", m_settings.isGridHide);
    settings.setValue("hideStarSky", m_settings.isStarSkyHide);
    settings.setValue("starsNumber", m_settings.starsNumber);
    settings.endGroup();
}

void FieldSettingsWidget::loadSettings() {
    spdlog::info("Загрузка параметров сетки");

    QSettings settings("config.ini", QSettings::IniFormat);
    if(!settings.childGroups().contains("GRID PARAMETERS") ||
        !settings.childGroups().contains("SEGMENT PARAMETERS") ||
        !settings.childGroups().contains("OPTIONS"))
    {
        spdlog::info("Сохранение параметров сетки при первом запуске");
        saveSettings();
    }
    else {
        settings.beginGroup("GRID PARAMETERS");
        m_settings.minX = settings.value("minX").toDouble();
        m_settings.minY = settings.value("minY").toDouble();
        m_settings.minZ = settings.value("minZ").toDouble();
        m_settings.maxX = settings.value("maxX").toDouble();
        m_settings.maxY = settings.value("maxY").toDouble();
        m_settings.maxZ = settings.value("maxZ").toDouble();
        settings.endGroup();

        settings.beginGroup("SEGMENT PARAMETERS");
        m_settings.segmentCountX = settings.value("stepX").toUInt();
        m_settings.segmentCountY = settings.value("stepY").toUInt();
        m_settings.segmentCountZ = settings.value("stepZ").toUInt();
        settings.endGroup();

        settings.beginGroup("OPTIONS");
        m_settings.isGridHide = settings.value("hideGrid").toBool();
        m_settings.isStarSkyHide = settings.value("hideStarSky").toBool();
        m_settings.starsNumber = settings.value("starsNumber").toUInt();
        settings.endGroup();
    }
}

void FieldSettingsWidget::updateParameters() {
    spdlog::info("Обновление параметров сетки в гуи");

    ui->SB_minX->setValue(m_settings.minX);
    ui->SB_minY->setValue(m_settings.minY);
    ui->SB_minZ->setValue(m_settings.minZ);

    ui->SB_maxX->setValue(m_settings.maxX);
    ui->SB_maxY->setValue(m_settings.maxY);
    ui->SB_maxZ->setValue(m_settings.maxZ);

    ui->SB_stepX->setValue(m_settings.segmentCountX);
    ui->SB_stepY->setValue(m_settings.segmentCountY);
    ui->SB_stepZ->setValue(m_settings.segmentCountZ);

    ui->CB_hideGrid->setChecked(m_settings.isGridHide);
    ui->CB_hideStarSky->setChecked(m_settings.isStarSkyHide);

    ui->SB_stars->setValue(m_settings.starsNumber);
}

GridSettings FieldSettingsWidget::settings() const
{
    return m_settings;
}
