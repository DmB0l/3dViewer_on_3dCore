#include <QApplication>
#include <QGuiApplication>

#include "mainwindow.h"
#include "view3d.h"

int main(int argc, char *argv[])
{
//     {
//         // Set OpenGL requirements
//         QSurfaceFormat format = QSurfaceFormat::defaultFormat();
// #ifndef QT_OPENGL_ES_2
//         format.setVersion(4, 1);
//         format.setProfile(QSurfaceFormat::CoreProfile);
//         format.setSamples(4);
// #else
//         format.setVersion(3, 0);
//         format.setProfile(QSurfaceFormat::NoProfile);
//         format.setRenderableType(QSurfaceFormat::OpenGLES);
// #endif
//         QSurfaceFormat::setDefaultFormat(format);
//     }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    qRegisterMetaType<QVector<Qt3DCore::QEntity*>>("QVector<Qt3DCore::QEntity*>");
    // qmlRegisterType<View3d>("View3dModule", 1, 0, "View3dComponent");

    MainWindow mainWindow;
    mainWindow.show();
    // Worker worker;
    return a.exec();
}
