#include "imagebutton.h"

#include <QMouseEvent>
#include <QTimer>
#include <QToolTip>
#include <QCursor>
#include <QGraphicsOpacityEffect>

ImageButton::ImageButton(QWidget *parent)
    : QToolButton(parent),
      m_timer(new QTimer(this)),
      m_effect(new QGraphicsOpacityEffect(this))
{
    setAutoRaise(true);
    setCursor(Qt::PointingHandCursor);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setIconSize(QSize(110, 110));

    setStyleSheet(
        "QToolButton {"
        "  border: none;"
        "  background: transparent;"
        "  color: white;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QToolButton:hover {"
        "  background: transparent;"
        "}");

    setGraphicsEffect(m_effect);

    connect(m_timer, &QTimer::timeout, this, &ImageButton::onTick);
    refreshVisual();
    setToolTip("可点击");
}

void ImageButton::setButtonImage(const QString &filePath)
{
    setIcon(QIcon(filePath));
}

void ImageButton::startCooldown(int seconds)
{
    if (seconds <= 0)
    {
        return;
    }

    m_endTime = QDateTime::currentDateTime().addSecs(seconds);
    m_timer->start(1000);
    refreshVisual();
    setToolTip(statusText());
}

int ImageButton::remainingSeconds() const
{
    if (!m_endTime.isValid())
    {
        return 0;
    }

    int left = QDateTime::currentDateTime().secsTo(m_endTime);
    return left > 0 ? left : 0;
}

bool ImageButton::ready() const
{
    return remainingSeconds() == 0;
}

QString ImageButton::statusText() const
{
    int left = remainingSeconds();
    if (left <= 0)
    {
        return "可点击";
    }
    return QString("剩余冷却时间：%1秒").arg(left);
}

void ImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !ready())
    {
        QToolTip::showText(QCursor::pos(), statusText(), this);
        event->accept();
        return;
    }

    QToolButton::mouseReleaseEvent(event);
}

void ImageButton::enterEvent(QEvent *event)
{
    QToolTip::showText(QCursor::pos(), statusText(), this);
    QToolButton::enterEvent(event);
}

void ImageButton::leaveEvent(QEvent *event)
{
    QToolTip::hideText();
    QToolButton::leaveEvent(event);
}

void ImageButton::onTick()
{
    if (remainingSeconds() <= 0)
    {
        m_timer->stop();
        m_endTime = QDateTime();
    }

    refreshVisual();
    setToolTip(statusText());
}

void ImageButton::refreshVisual()
{
    bool isReady = ready();
    m_effect->setOpacity(isReady ? 1.0 : 0.55);
    setCursor(isReady ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
}
