#include "colorsettingswidget.h"
#include "ui_colorsettingswidget.h"

ColorSettingsWidget::ColorSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorSettingsWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Color settings");

    loadSettings();

    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        connect(okButton, &QPushButton::clicked, this, [=]() {
            saveSettings();
            emit updateColorSettings();
            // ui->PB_chooseBackgroundColor->setStyleSheet("background-color: " + m_backgroundColor.name() + ";");
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
            // ui->PB_chooseBackgroundColor->setStyleSheet("background-color: " + m_backgroundColor.name() + ";");
            emit updateColorSettings();
        });
    }

    ui->PB_chooseBackgroundColor->setStyleSheet("background-color: " + m_backgroundColor.name() + ";");

    connect(ui->PB_chooseBackgroundColor, &QAbstractButton::clicked, this, &ColorSettingsWidget::pickColor);
}

ColorSettingsWidget::~ColorSettingsWidget()
{
    delete ui;
}

void ColorSettingsWidget::closeEvent(QCloseEvent *event) {
    emit closeSignal();
    QWidget::closeEvent(event); // Вызов базового класса
}

void ColorSettingsWidget::pickColor(bool checked) {
    // Открываем диалог выбора цвета
    m_backgroundColor = QColorDialog::getColor(m_backgroundColor, this, "Select Color", QColorDialog::DontUseNativeDialog);
    ui->PB_chooseBackgroundColor->setStyleSheet("background-color: " + m_backgroundColor.name() + ";");
}

void ColorSettingsWidget::saveSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("COLOR PARAMETERS");
    settings.setValue("backgroundColor", m_backgroundColor.name());
    settings.endGroup();
}

QColor ColorSettingsWidget::backgroundColor() const {
    return m_backgroundColor;
}

void ColorSettingsWidget::loadSettings() {
    QSettings settings("config.ini", QSettings::IniFormat);
    if(!settings.childGroups().contains("COLOR PARAMETERS")) {
        saveSettings();
    }
    else {
        settings.beginGroup("COLOR PARAMETERS");
        m_backgroundColor = QColor(settings.value("backgroundColor").toString());
        settings.endGroup();
    }
}
