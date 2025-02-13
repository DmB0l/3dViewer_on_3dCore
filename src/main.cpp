#include <QApplication>
#include <QGuiApplication>

#include "mainwindow.h"

#define MAX_LOG_COUNT 5

void initializeLogger() {
    QDateTime now = QDateTime::currentDateTime();

    QString filename = QString("logs/%1-%2-%3_%4-%5.log")
                           .arg(now.date().year(), 4, 10, QLatin1Char('0')) // Год
                           .arg(now.date().month(), 2, 10, QLatin1Char('0')) // Месяц
                           .arg(now.date().day(), 2, 10, QLatin1Char('0'))   // День
                           .arg(now.time().hour(), 2, 10, QLatin1Char('0'))  // Час
                           .arg(now.time().minute(), 2, 10, QLatin1Char('0')); // Минута

    QDir dir;
    if (!dir.exists("logs")) {
        dir.mkpath("logs");
    }
    dir.cd("logs");
    dir.setNameFilters(QStringList("*.log"));

    QFileInfoList fileNames = dir.entryInfoList();
    for(int i = 0; i <= fileNames.size() - MAX_LOG_COUNT; i++) {
        QFile::remove(fileNames[i].absoluteFilePath());
    }

    auto file_logger = spdlog::basic_logger_mt("file_logger", filename.toStdString());

    spdlog::set_default_logger(file_logger);
}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    qRegisterMetaType<QVector<Qt3DCore::QEntity*>>("QVector<Qt3DCore::QEntity*>");

    initializeLogger();
    spdlog::info("Start program");

    qDebug() << QColor::colorNames();

    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
