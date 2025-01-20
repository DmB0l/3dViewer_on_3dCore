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
    m_minX = ui->SB_minX->value();
    m_minY = ui->SB_minY->value();
    m_minZ = ui->SB_minZ->value();

    m_maxX = ui->SB_maxX->value();
    m_maxY = ui->SB_maxY->value();
    m_maxZ = ui->SB_maxZ->value();

    m_segmentCountX = ui->SB_stepX->value();
    m_segmentCountY = ui->SB_stepY->value();
    m_segmentCountZ = ui->SB_stepZ->value();

    m_isGridHide = ui->CB_hideGrid->isChecked();

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("GRID PARAMETERS");
    settings.setValue("minX", m_minX);
    settings.setValue("minY", m_minY);
    settings.setValue("minZ", m_minZ);

    settings.setValue("maxX", m_maxX);
    settings.setValue("maxY", m_maxY);
    settings.setValue("maxZ", m_maxZ);
    settings.endGroup();

    settings.beginGroup("SEGMENT PARAMETERS");
    settings.setValue("stepX", m_segmentCountX);
    settings.setValue("stepY", m_segmentCountY);
    settings.setValue("stepZ", m_segmentCountZ);
    settings.endGroup();

    settings.beginGroup("HIDE GRID");
    settings.setValue("hideGrid", m_isGridHide);
    settings.endGroup();
}

void FieldSettingsWidget::loadSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    if(settings.childGroups().isEmpty()) {
        saveSettings();
    }
    else {
        settings.beginGroup("GRID PARAMETERS");
        m_minX = settings.value("minX").toDouble();
        m_minY = settings.value("minY").toDouble();
        m_minZ = settings.value("minZ").toDouble();
        m_maxX = settings.value("maxX").toDouble();
        m_maxY = settings.value("maxY").toDouble();
        m_maxZ = settings.value("maxZ").toDouble();
        settings.endGroup();

        settings.beginGroup("SEGMENT PARAMETERS");
        m_segmentCountX = settings.value("stepX").toUInt();
        m_segmentCountY = settings.value("stepY").toUInt();
        m_segmentCountZ = settings.value("stepZ").toUInt();
        settings.endGroup();

        settings.beginGroup("HIDE GRID");
        m_isGridHide = settings.value("hideGrid").toBool();
        settings.endGroup();
    }
}

void FieldSettingsWidget::updateParameters() {
    ui->SB_minX->setValue(m_minX);
    ui->SB_minY->setValue(m_minY);
    ui->SB_minZ->setValue(m_minZ);

    ui->SB_maxX->setValue(m_maxX);
    ui->SB_maxY->setValue(m_maxY);
    ui->SB_maxZ->setValue(m_maxZ);

    ui->SB_stepX->setValue(m_segmentCountX);
    ui->SB_stepY->setValue(m_segmentCountY);
    ui->SB_stepZ->setValue(m_segmentCountZ);

    ui->CB_hideGrid->setChecked(m_isGridHide);
}

bool FieldSettingsWidget::isGridHide() const
{
    return m_isGridHide;
}

double FieldSettingsWidget::minX() const
{
    return m_minX;
}

double FieldSettingsWidget::minY() const
{
    return m_minY;
}

double FieldSettingsWidget::minZ() const
{
    return m_minZ;
}

double FieldSettingsWidget::maxX() const
{
    return m_maxX;
}

double FieldSettingsWidget::maxY() const
{
    return m_maxY;
}

double FieldSettingsWidget::maxZ() const
{
    return m_maxZ;
}

uint32_t FieldSettingsWidget::segmentCountX() const
{
    return m_segmentCountX;
}

uint32_t FieldSettingsWidget::segmentCountY() const
{
    return m_segmentCountY;
}

uint32_t FieldSettingsWidget::segmentCountZ() const
{
    return m_segmentCountZ;
}
