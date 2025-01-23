#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QMouseEvent>
#include <QAbstractSlider>
#include <QtMath>
#include <QDebug>
#include <QWheelEvent>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QWidget>
#include <QGraphicsPixmapItem>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *mainWidget, QWidget *parent = nullptr);


private:
    QGraphicsScene *m_scene = nullptr; // Сцена для отображения изображения
    QLabel *m_labelCameraNumberOverlay = nullptr; // Номер камеры
    QPushButton *m_buttonOverlay = nullptr; // Кнопка раскрытия информации о камере
    QWidget* m_infoWidget = nullptr; // Widget для отображения информации о камере
    QGridLayout *m_gridLayoutInfoWidget = nullptr; // Layout
    QLabel *m_labelCameraNumber = nullptr;
    QGridLayout *m_gridLayoutInfoButton = nullptr; // Layout

    QWidget* m_mainWidget = nullptr; // Widget для отображения информации о камере
};

#endif // CUSTOMGRAPHICSVIEW_H
