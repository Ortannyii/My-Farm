#ifndef GAMECANVASWIDGET_H
#define GAMECANVASWIDGET_H

#include <QWidget>
#include <QPixmap>

class GameCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameCanvasWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_bg;
};

#endif
