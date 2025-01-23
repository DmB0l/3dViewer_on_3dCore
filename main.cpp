#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QVector<Qt3DCore::QEntity*>>("QVector<Qt3DCore::QEntity*>");

    MainWindow mainWindow;
    mainWindow.show();
    // Worker worker;
    return a.exec();
}
