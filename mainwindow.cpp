#include "mainwindow.h"

#include "gamecanvaswidget.h"
#include "farmlandwidget.h"
#include "imagebutton.h"
#include "actiondialog.h"

#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_money(5000),
      m_trophyUnlocked(false),
      m_canvas(new GameCanvasWidget(this)),
      m_moneyLabel(nullptr),
      m_trophyLabel(nullptr),
      m_hoverLabel(nullptr),
      m_rainOverlay(nullptr),
      m_rainButton(nullptr),
      m_blessButton(nullptr),
      m_bgmPlayer(nullptr),
      m_bgmList(nullptr),
      m_rainPlayer(nullptr),
      m_moneyPlayer(nullptr)
{
    setWindowTitle("My Farm");
    setFixedSize(1920, 1080);
    setCentralWidget(m_canvas);

    createTopUi();
    createBottomUi();
    createLandWidgets();
    initSounds();

    refreshMoneyLabel();
    hideHoverInfo();
}

QWidget *MainWindow::canvas() const
{
    return m_canvas;
}

void MainWindow::createTopUi()
{
    m_moneyLabel = new QLabel(m_canvas);
    m_moneyLabel->setGeometry(20, 20, 400, 40);
    m_moneyLabel->setStyleSheet(
        "QLabel {"
        "  color: white;"
        "  font-size: 26px;"
        "  font-weight: bold;"
        "  background: rgba(0,0,0,120);"
        "  padding-left: 12px;"
        "  padding-right: 12px;"
        "  border-radius: 8px;"
        "}");

    m_trophyLabel = new QLabel(m_canvas);
    m_trophyLabel->setGeometry(1820, 20, 80, 80);
    m_trophyLabel->setPixmap(QPixmap(":/images/resources/trophy.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_trophyLabel->setAlignment(Qt::AlignCenter);
    m_trophyLabel->hide();

    m_hoverLabel = new QLabel(m_canvas);
    m_hoverLabel->setStyleSheet(
        "QLabel {"
        "  color: white;"
        "  background: rgba(0,0,0,180);"
        "  border: 1px solid rgba(255,255,255,120);"
        "  border-radius: 6px;"
        "  padding: 6px 10px;"
        "  font-size: 18px;"
        "}");
    m_hoverLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_hoverLabel->hide();

    m_rainOverlay = new QLabel(m_canvas);
    m_rainOverlay->setGeometry(0, 0, 1920, 1080);
    m_rainOverlay->setPixmap(QPixmap(":/images/resources/rain_scene.png").scaled(1920, 1080, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    m_rainOverlay->setScaledContents(true);
    m_rainOverlay->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_rainOverlay->hide();
    m_rainOverlay->raise();

    m_moneyLabel->raise();
    m_trophyLabel->raise();
    m_hoverLabel->raise();
}

void MainWindow::createBottomUi()
{
    m_blessButton = new ImageButton(m_canvas);
    m_blessButton->setGeometry(30, 900, 170, 170);
    m_blessButton->setButtonImage(":/images/resources/btn_pray.png");
    m_blessButton->setText("祈福");
    connect(m_blessButton, &QToolButton::clicked, this, &MainWindow::onBlessClicked);

    m_rainButton = new ImageButton(m_canvas);
    m_rainButton->setGeometry(1720, 900, 170, 170);
    m_rainButton->setButtonImage(":/images/resources/btn_rain.png");
    m_rainButton->setText("求雨");
    connect(m_rainButton, &QToolButton::clicked, this, &MainWindow::onRainClicked);

    m_blessButton->raise();
    m_rainButton->raise();
}

void MainWindow::createLandWidgets()
{
    const int startX = 330;
    const int startY = 177;
    const int totalW = 1280;
    const int totalH = 723;
    const int rows = 3;
    const int cols = 5;

    const int cellW = totalW / cols;
    const int cellH = totalH / rows;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int x = startX + c * cellW;
            int y = startY + r * cellH;

            int w = cellW;
            int h = cellH;

            if (c == cols - 1)
            {
                w = totalW - cellW * (cols - 1);
            }
            if (r == rows - 1)
            {
                h = totalH - cellH * (rows - 1);
            }

            FarmLandWidget *land = new FarmLandWidget(r + 1, c + 1, m_canvas);
            land->setGeometry(x, y, w, h);

            connect(land, &FarmLandWidget::landClicked, this, &MainWindow::onLandClicked);
            connect(land, &FarmLandWidget::hoverInfoRequested, this, &MainWindow::showHoverInfo);
            connect(land, &FarmLandWidget::hoverLeft, this, &MainWindow::hideHoverInfo);

            m_lands.append(land);
            land->raise();
        }
    }

    m_rainOverlay->raise();
    m_moneyLabel->raise();
    m_trophyLabel->raise();
    m_hoverLabel->raise();
    m_blessButton->raise();
    m_rainButton->raise();
}

void MainWindow::initSounds()
{
    m_bgmList = new QMediaPlaylist(this);
    m_bgmList->addMedia(QUrl("qrc:/audio/resources/bgm.wav"));
    m_bgmList->setPlaybackMode(QMediaPlaylist::Loop);

    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmPlayer->setPlaylist(m_bgmList);
    m_bgmPlayer->setVolume(50);
    m_bgmPlayer->play();

    m_rainPlayer = new QMediaPlayer(this);
    m_rainPlayer->setMedia(QUrl("qrc:/audio/resources/rain.wav"));
    m_rainPlayer->setVolume(75);

    m_moneyPlayer = new QMediaPlayer(this);
    m_moneyPlayer->setMedia(QUrl("qrc:/audio/resources/money.wav"));
    m_moneyPlayer->setVolume(75);
}

void MainWindow::refreshMoneyLabel()
{
    m_moneyLabel->setText(QString("货币：$%1").arg(m_money));

    if (m_money >= 10000)
    {
        m_trophyUnlocked = true;
    }

    updateTrophy();
}

void MainWindow::updateTrophy()
{
    if (m_trophyUnlocked)
    {
        m_trophyLabel->show();
        m_trophyLabel->raise();
    }
}

void MainWindow::showHoverInfo(const QString &text, const QPoint &globalPos)
{
    m_hoverLabel->setText(text);
    m_hoverLabel->adjustSize();

    QPoint localPos = m_canvas->mapFromGlobal(globalPos + QPoint(18, 18));
    int x = localPos.x();
    int y = localPos.y();

    if (x + m_hoverLabel->width() > m_canvas->width())
    {
        x = m_canvas->width() - m_hoverLabel->width() - 10;
    }
    if (y + m_hoverLabel->height() > m_canvas->height())
    {
        y = m_canvas->height() - m_hoverLabel->height() - 10;
    }

    if (x < 0) x = 0;
    if (y < 0) y = 0;

    m_hoverLabel->move(x, y);
    m_hoverLabel->show();
    m_hoverLabel->raise();
}

void MainWindow::hideHoverInfo()
{
    m_hoverLabel->hide();
}

void MainWindow::onLandClicked(FarmLandWidget *land)
{
    if (!land)
    {
        return;
    }

    ActionDialog *dlg = new ActionDialog(this, land, this);
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

void MainWindow::plantCrop(FarmLandWidget *land, CropType type)
{
    if (!land)
    {
        return;
    }

    int price = 0;
    if (type == CropType::Wheat)
    {
        price = 100;
    }
    else if (type == CropType::Rice)
    {
        price = 200;
    }

    if (m_money < price)
    {
        QMessageBox::information(this, "提示", "货币不足");
        return;
    }

    if (!land->plantCrop(type))
    {
        return;
    }

    m_money -= price;
    refreshMoneyLabel();
}

void MainWindow::waterCrop(FarmLandWidget *land)
{
    if (!land)
    {
        return;
    }

    land->waterCrop();
}

void MainWindow::fertilizeCrop(FarmLandWidget *land)
{
    if (!land)
    {
        return;
    }

    land->fertilizeCrop();
}

void MainWindow::removeCrop(FarmLandWidget *land)
{
    if (!land)
    {
        return;
    }

    land->removeCrop();
}

void MainWindow::sellCrop(FarmLandWidget *land)
{
    if (!land || !land->hasCrop())
    {
        return;
    }

    int gain = land->sellPrice();
    if (gain <= 0)
    {
        return;
    }

    m_money += gain;
    land->removeCrop();
    refreshMoneyLabel();
}

void MainWindow::onRainClicked()
{
    if (m_rainButton && !m_rainButton->ready())
    {
        return;
    }

    if (m_rainButton)
    {
        m_rainButton->startCooldown(40);
    }

    if (m_rainPlayer)
    {
        m_rainPlayer->stop();
        m_rainPlayer->play();
    }

    m_rainOverlay->show();
    m_rainOverlay->raise();

    int affected = 0;
    for (FarmLandWidget *land : m_lands)
    {
        if (land->hasCrop())
        {
            land->rainWaterPlusOne();
            ++affected;
        }
    }

    QTimer::singleShot(5000, this, [this]()
    {
        m_rainOverlay->hide();
        if (m_rainPlayer)
        {
            m_rainPlayer->stop();
        }
    });

    QMessageBox::information(this, "提示", QString("东风谷早苗为你求来了一场大雨！%1片作物得到了雨水的滋润。").arg(affected));
}

void MainWindow::onBlessClicked()
{
    if (m_blessButton && !m_blessButton->ready())
    {
        return;
    }

    if (m_blessButton)
    {
        m_blessButton->startCooldown(80);
    }

    if (m_moneyPlayer)
    {
        m_moneyPlayer->stop();
        m_moneyPlayer->play();
    }

    int addMoney = QRandomGenerator::global()->bounded(101);
    m_money += addMoney;
    refreshMoneyLabel();

    QMessageBox::information(this, "提示", QString("博丽灵梦给你送来了$%1！").arg(addMoney));
}
