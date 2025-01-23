#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *mainWidget, QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);

    this->setStyleSheet("QGraphicsView {"
                        "background-color: rgba(255, 255, 255, 0);"
                        "border: none;"
                        "}"
                        "QGraphicsView:hover {"
                        "background-color: rgba(255, 255, 255, 50);"
                        "}"
                        "QGraphicsView:pressed {"
                        "background-color: rgba(255, 255, 255, 100);"
                        "}"
                        );

    m_mainWidget = mainWidget;
    m_mainWidget->setParent(this);
    m_mainWidget->setGeometry(0,0,300,330);
    m_mainWidget->setStyleSheet("background-color: white");
    // m_scene->addWidget(m_mainWidget);

    m_labelCameraNumberOverlay = new QLabel(this);
    m_labelCameraNumberOverlay->setText(QString::number(1));
    m_labelCameraNumberOverlay->setStyleSheet("background-color: rgba(255, 255, 0, 100); border: 1px solid red;");
    m_labelCameraNumberOverlay->setFixedSize(50, 50);
    m_labelCameraNumberOverlay->setMargin(0);

    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_buttonOverlay = new QPushButton("Info", this);
    m_buttonOverlay->setStyleSheet("QPushButton {"
                                   "background-color: rgba(255, 255, 255, 100);"
                                   "border: none;"
                                   "color: black;"
                                   "font-size: 12px;"
                                   "border-radius: 1px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: rgba(255, 255, 255, 150);"
                                   "}"
                                   "QPushButton:pressed {"
                                   "background-color: rgba(255, 255, 255, 200);"
                                   "}");
    m_buttonOverlay->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_buttonOverlay->setFixedSize(33, 15);

    m_infoWidget = new QWidget(this);
    m_infoWidget->setVisible(true);
    m_infoWidget->setStyleSheet("background-color: rgba(255, 255, 255, 50);"
                                "border: none;"
                                "color: black;"
                                "font-size: 12px;"
                                // "font-weight: bold;"
                                "border-radius: 1px;"
                                );

    m_gridLayoutInfoWidget = new QGridLayout(m_infoWidget);
    m_gridLayoutInfoWidget->setContentsMargins(0, 0, 10, 0); // Убираем отступы
    m_gridLayoutInfoWidget->setSpacing(0); // Убираем расстояние между элементами

    m_labelCameraNumber = new QLabel(this);
    m_gridLayoutInfoWidget->addWidget(m_labelCameraNumber, 0, 0);

    // m_infoWidget->installEventFilter(this);

    m_gridLayoutInfoButton = new QGridLayout(this);
    m_gridLayoutInfoButton->setContentsMargins(0, 0, 10, 0);
    m_gridLayoutInfoButton->setSpacing(0);

    m_gridLayoutInfoButton->addWidget(m_buttonOverlay, 0, 0, Qt::AlignRight | Qt::AlignTop);
    m_gridLayoutInfoButton->addWidget(m_infoWidget, 1, 0, Qt::AlignRight | Qt::AlignTop);

    m_gridLayoutInfoButton->setRowStretch(1, 1);
}
