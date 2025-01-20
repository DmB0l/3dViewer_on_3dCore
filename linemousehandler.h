#ifndef LINEMOUSEHANDLER_H
#define LINEMOUSEHANDLER_H

#include <QObject>
#include <Qt3DInput/QMouseHandler>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

class LineMouseHandler : public Qt3DInput::QMouseHandler {
    Q_OBJECT

public:
    LineMouseHandler(Qt3DCore::QEntity *parent = nullptr);

private slots:
    void onMousePressed(Qt3DInput::QMouseEvent *event);

    void onMouseReleased(Qt3DInput::QMouseEvent *event);

    // QObject interface
public:
    bool event(QEvent *event) override {
        qDebug() << event->type();
        return QMouseHandler::event(event);
    }
};

#endif // LINEMOUSEHANDLER_H
