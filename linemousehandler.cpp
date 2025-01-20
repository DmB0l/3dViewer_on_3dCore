#include "linemousehandler.h"

LineMouseHandler::LineMouseHandler(Qt3DCore::QEntity *parent) : Qt3DInput::QMouseHandler(parent) {
    connect(this, &Qt3DInput::QMouseHandler::pressed, this, &LineMouseHandler::onMousePressed);
    connect(this, &Qt3DInput::QMouseHandler::released, this, &LineMouseHandler::onMouseReleased);
}

void LineMouseHandler::onMousePressed(Qt3DInput::QMouseEvent *event) {
    if(event->button() == Qt3DInput::QMouseEvent::LeftButton) {
        qDebug() << "Mouse pressed at x:" << event->x() << "y:" << event->y();
    }
}

void LineMouseHandler::onMouseReleased(Qt3DInput::QMouseEvent *event) {
    qDebug() << "Mouse released at x:" << event->x() << "y:" << event->y();
}
