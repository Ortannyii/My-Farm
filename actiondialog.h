#ifndef ACTIONDIALOG_H
#define ACTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include "gametypes.h"

class MainWindow;
class FarmLandWidget;
class ImageButton;
class QLabel;

class ActionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ActionDialog(MainWindow *owner, FarmLandWidget *land, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onPlantWheat();
    void onPlantRice();
    void onWater();
    void onFertilize();
    void onRemove();
    void onSell();

private:
    void rebuildButtons();
    void clearButtons();

private:
    MainWindow *m_owner;
    FarmLandWidget *m_land;

    QLabel *m_titleLabel;
    QVector<ImageButton *> m_buttons;

    QPixmap m_bg;
};

#endif
