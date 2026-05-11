#ifndef FARMLANDWIDGET_H
#define FARMLANDWIDGET_H

#include <QWidget>
#include "gametypes.h"

class FarmLandWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FarmLandWidget(int row, int col, QWidget *parent = nullptr);

    int rowIndex() const;
    int colIndex() const;

    bool hasCrop() const;
    bool isMature() const;
    CropType cropType() const;
    int waterCount() const;
    int fertilizerCount() const;

    bool plantCrop(CropType type);
    void waterCrop();
    void fertilizeCrop();
    void removeCrop();
    void rainWaterPlusOne();

    int sellPrice() const;
    QString hoverText() const;

signals:
    void landClicked(FarmLandWidget *land);
    void hoverInfoRequested(const QString &text, const QPoint &globalPos);
    void hoverLeft();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_row;
    int m_col;

    CropType m_cropType;
    int m_waterCount;
    int m_fertilizerCount;
};

#endif
