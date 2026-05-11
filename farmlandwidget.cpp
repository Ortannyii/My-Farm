#include "farmlandwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

FarmLandWidget::FarmLandWidget(int row, int col, QWidget *parent)
    : QWidget(parent),
      m_row(row),
      m_col(col),
      m_cropType(CropType::None),
      m_waterCount(0),
      m_fertilizerCount(0)
{
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

int FarmLandWidget::rowIndex() const
{
    return m_row;
}

int FarmLandWidget::colIndex() const
{
    return m_col;
}

bool FarmLandWidget::hasCrop() const
{
    return m_cropType != CropType::None;
}

bool FarmLandWidget::isMature() const
{
    if (!hasCrop())
    {
        return false;
    }
    return m_waterCount >= 10 && m_fertilizerCount >= 5;
}

CropType FarmLandWidget::cropType() const
{
    return m_cropType;
}

int FarmLandWidget::waterCount() const
{
    return m_waterCount;
}

int FarmLandWidget::fertilizerCount() const
{
    return m_fertilizerCount;
}

bool FarmLandWidget::plantCrop(CropType type)
{
    if (hasCrop())
    {
        return false;
    }

    m_cropType = type;
    m_waterCount = 0;
    m_fertilizerCount = 0;
    update();
    return true;
}

void FarmLandWidget::waterCrop()
{
    if (!hasCrop())
    {
        return;
    }

    m_waterCount++;
    update();
}

void FarmLandWidget::fertilizeCrop()
{
    if (!hasCrop())
    {
        return;
    }

    m_fertilizerCount++;
    update();
}

void FarmLandWidget::removeCrop()
{
    m_cropType = CropType::None;
    m_waterCount = 0;
    m_fertilizerCount = 0;
    update();
}

void FarmLandWidget::rainWaterPlusOne()
{
    if (!hasCrop())
    {
        return;
    }

    m_waterCount++;
    update();
}

int FarmLandWidget::sellPrice() const
{
    if (!hasCrop())
    {
        return 0;
    }

    if (m_cropType == CropType::Wheat)
    {
        return 150;
    }

    if (m_cropType == CropType::Rice)
    {
        return 300;
    }

    return 0;
}

QString FarmLandWidget::hoverText() const
{
    if (!hasCrop())
    {
        return "无作物";
    }

    QString text;
    text += "作物：" + cropName(m_cropType) + "\n";
    text += "成熟状态：" + QString(isMature() ? "已成熟" : "未成熟") + "\n";
    text += "浇水次数：" + QString::number(m_waterCount) + "\n";
    text += "施肥次数：" + QString::number(m_fertilizerCount);
    return text;
}

void FarmLandWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if (!hasCrop())
    {
        return;
    }

    QString imgPath;
    if (m_cropType == CropType::Wheat)
    {
        imgPath = ":/images/resources/crop_wheat.png";
    }
    else if (m_cropType == CropType::Rice)
    {
        imgPath = ":/images/resources/crop_rice.png";
    }

    QPixmap pix(imgPath);
    if (pix.isNull())
    {
        return;
    }

    QSize targetSize = pix.size();
    targetSize.scale(width() * 0.72, height() * 0.72, Qt::KeepAspectRatio);

    QRect targetRect(
        (width() - targetSize.width()) / 2,
        (height() - targetSize.height()) / 2 - 5,
        targetSize.width(),
        targetSize.height());

    painter.drawPixmap(targetRect, pix.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void FarmLandWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit landClicked(this);
    }

    QWidget::mousePressEvent(event);
}

void FarmLandWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    emit hoverInfoRequested(hoverText(), QCursor::pos());
}

void FarmLandWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    emit hoverLeft();
}

void FarmLandWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit hoverInfoRequested(hoverText(), QCursor::pos());
    QWidget::mouseMoveEvent(event);
}
