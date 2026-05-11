#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "gametypes.h"

class GameCanvasWidget;
class FarmLandWidget;
class QLabel;
class ImageButton;
class QMediaPlayer;
class QMediaPlaylist;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void plantCrop(FarmLandWidget *land, CropType type);
    void waterCrop(FarmLandWidget *land);
    void fertilizeCrop(FarmLandWidget *land);
    void removeCrop(FarmLandWidget *land);
    void sellCrop(FarmLandWidget *land);

    QWidget *canvas() const;

public slots:
    void showHoverInfo(const QString &text, const QPoint &globalPos);
    void hideHoverInfo();

private slots:
    void onLandClicked(FarmLandWidget *land);
    void onRainClicked();
    void onBlessClicked();

private:
    void createLandWidgets();
    void createTopUi();
    void createBottomUi();
    void initSounds();
    void refreshMoneyLabel();
    void updateTrophy();

private:
    int m_money;
    bool m_trophyUnlocked;

    GameCanvasWidget *m_canvas;

    QLabel *m_moneyLabel;
    QLabel *m_trophyLabel;
    QLabel *m_hoverLabel;
    QLabel *m_rainOverlay;

    ImageButton *m_rainButton;
    ImageButton *m_blessButton;

    QVector<FarmLandWidget *> m_lands;

    QMediaPlayer *m_bgmPlayer;
    QMediaPlaylist *m_bgmList;
    QMediaPlayer *m_rainPlayer;
    QMediaPlayer *m_moneyPlayer;
};

#endif
