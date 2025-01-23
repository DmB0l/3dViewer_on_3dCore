#include "camerasettingswidget.h"
#include "ui_camerasettingswidget.h"

CameraSettingsWidget::CameraSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CameraSettingsWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Grid settings");

    loadSettings();

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        connect(okButton, &QPushButton::clicked, this, [=]() {
            saveSettings();
            emit updateCameraSettings();
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
            emit updateCameraSettings();
        });
    }
}

CameraSettingsWidget::~CameraSettingsWidget() {
    delete ui;
}

void CameraSettingsWidget::closeEvent(QCloseEvent *event) {
    emit closeSignal();
    QWidget::closeEvent(event); // Вызов базового класса
}

void CameraSettingsWidget::showEvent(QShowEvent *event) {
    loadSettings();
    updateParameters();
    QWidget::showEvent(event); // Вызов базового класса
}

void CameraSettingsWidget::saveSettings() {
    m_settings.linearSpeed = ui->SB_linearSpeed->value();
    m_settings.lookSpeed = ui->SB_lookSpeed->value();
    m_settings.farPlane = ui->SB_farPlane->value();
    m_settings.nearPlane = ui->DSB_nearPlane->value();
    m_settings.fieldOfView = ui->SB_fieldOfView->value();

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("CAMERA PARAMETERS");
    settings.setValue("linearSpeed", m_settings.linearSpeed);
    settings.setValue("lookSpeed", m_settings.lookSpeed);
    settings.setValue("farPlane", m_settings.farPlane);
    settings.setValue("nearPlane", m_settings.nearPlane);
    settings.setValue("fieldOfView", m_settings.fieldOfView);
    settings.endGroup();
}

void CameraSettingsWidget::loadSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    if(!settings.childGroups().contains("CAMERA PARAMETERS")) {
        saveSettings();
    }
    else {
        settings.beginGroup("CAMERA PARAMETERS");
        m_settings.linearSpeed = settings.value("linearSpeed").toInt();
        m_settings.lookSpeed = settings.value("lookSpeed").toInt();
        m_settings.farPlane = settings.value("farPlane").toInt();
        m_settings.nearPlane = settings.value("nearPlane").toDouble();
        m_settings.fieldOfView = settings.value("fieldOfView").toInt();
        settings.endGroup();
    }
}

void CameraSettingsWidget::updateParameters() {
    ui->SB_linearSpeed->setValue(m_settings.linearSpeed);
    ui->SB_lookSpeed->setValue(m_settings.lookSpeed);
    ui->SB_farPlane->setValue(m_settings.farPlane);
    ui->DSB_nearPlane->setValue(m_settings.nearPlane);
    ui->SB_fieldOfView->setValue(m_settings.fieldOfView);
}

CameraSettings CameraSettingsWidget::cameraSettings() const {
    return m_settings;
}
