#include "gamecanvaswidget.h"
#include <QPainter>

GameCanvasWidget::GameCanvasWidget(QWidget *parent)
    : QWidget(parent),
      m_bg(":/images/resources/main_bg.png")
{
    setMouseTracking(true);
}

void GameCanvasWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_bg.isNull())
    {
        painter.drawPixmap(rect(), m_bg);
    }
    else
    {
        painter.fillRect(rect(), Qt::darkGreen);
    }
}
