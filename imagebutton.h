#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QToolButton>
#include <QDateTime>

class QTimer;
class QGraphicsOpacityEffect;

class ImageButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ImageButton(QWidget *parent = nullptr);

    void setButtonImage(const QString &filePath);
    void startCooldown(int seconds);
    int remainingSeconds() const;
    bool ready() const;

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void onTick();

private:
    QString statusText() const;
    void refreshVisual();

    QDateTime m_endTime;
    QTimer *m_timer;
    QGraphicsOpacityEffect *m_effect;
};

#endif
