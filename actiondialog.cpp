#include "actiondialog.h"

#include "mainwindow.h"
#include "farmlandwidget.h"
#include "imagebutton.h"

#include <QLabel>
#include <QPainter>
#include <QTimer>

ActionDialog::ActionDialog(MainWindow *owner, FarmLandWidget *land, QWidget *parent)
    : QDialog(parent),
      m_owner(owner),
      m_land(land),
      m_titleLabel(new QLabel(this)),
      m_bg(":/images/resources/popup_bg.png")
{
    setFixedSize(1366, 768);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_DeleteOnClose, true);

    if (m_land)
    {
        setWindowTitle(QString("土地 第%1行，第%2列").arg(m_land->rowIndex()).arg(m_land->colIndex()));
    }
    else
    {
        setWindowTitle("土地");
    }

    m_titleLabel->setGeometry(0, 30, 1366, 50);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "QLabel {"
        "  color: black;"
        "  font-size: 30px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "}");

    rebuildButtons();
}

void ActionDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    if (!m_bg.isNull())
    {
        painter.drawPixmap(rect(), m_bg);
    }
    else
    {
        painter.fillRect(rect(), Qt::darkGray);
    }
}

void ActionDialog::clearButtons()
{
    qDeleteAll(m_buttons);
    m_buttons.clear();
}

void ActionDialog::rebuildButtons()
{
    clearButtons();

    if (!m_land)
    {
        return;
    }

    const bool empty = !m_land->hasCrop();
    const bool mature = m_land->isMature();

    auto makeButton = [this](const QString &img, const QString &text, const QRect &geo) -> ImageButton *
    {
        ImageButton *btn = new ImageButton(this);
        btn->setGeometry(geo);
        btn->setButtonImage(img);
        btn->setText(text);
        btn->setIconSize(QSize(120, 120));
        btn->setStyleSheet(
            "QToolButton {"
            "  border: none;"
            "  background: transparent;"
            "  color: black;"
            "  font-size: 18px;"
            "  font-weight: bold;"
            "}"
            "QToolButton:hover {"
            "  background: rgba(255,255,255,20);"
            "  border-radius: 12px;"
            "}");
        btn->raise();
        m_buttons.append(btn);
        return btn;
    };

    if (empty)
    {
        ImageButton *wheatBtn = makeButton(":/images/resources/btn_wheat.png", "小麦\n$100", QRect(250, 400, 180, 180));
        ImageButton *riceBtn  = makeButton(":/images/resources/btn_rice.png", "水稻\n$200", QRect(930, 400, 180, 180));

        connect(wheatBtn, &QToolButton::clicked, this, &ActionDialog::onPlantWheat);
        connect(riceBtn, &QToolButton::clicked, this, &ActionDialog::onPlantRice);
    }
    else if (mature)
    {
        ImageButton *sellBtn   = makeButton(":/images/resources/btn_sell.png", QString("卖掉\n获得$%1").arg(m_land->sellPrice()), QRect(320, 400, 180, 180));
        ImageButton *removeBtn = makeButton(":/images/resources/btn_remove.png", "铲除", QRect(860, 400, 180, 180));

        connect(sellBtn, &QToolButton::clicked, this, &ActionDialog::onSell);
        connect(removeBtn, &QToolButton::clicked, this, &ActionDialog::onRemove);
    }
    else
    {
        ImageButton *waterBtn  = makeButton(":/images/resources/btn_water.png", "浇水", QRect(170, 400, 180, 180));
        ImageButton *fertBtn   = makeButton(":/images/resources/btn_fertilize.png", "施肥", QRect(593, 400, 180, 180));
        ImageButton *removeBtn = makeButton(":/images/resources/btn_remove.png", "铲除", QRect(1016, 400, 180, 180));

        connect(waterBtn, &QToolButton::clicked, this, &ActionDialog::onWater);
        connect(fertBtn, &QToolButton::clicked, this, &ActionDialog::onFertilize);
        connect(removeBtn, &QToolButton::clicked, this, &ActionDialog::onRemove);
    }

    m_titleLabel->raise();
}

void ActionDialog::onPlantWheat()
{
    if (m_owner && m_land)
    {
        m_owner->plantCrop(m_land, CropType::Wheat);
        if (m_land->hasCrop())
        {
            close();
        }
    }
}

void ActionDialog::onPlantRice()
{
    if (m_owner && m_land)
    {
        m_owner->plantCrop(m_land, CropType::Rice);
        if (m_land->hasCrop())
        {
            close();
        }
    }
}

void ActionDialog::onWater()
{
    if (m_owner && m_land)
    {
        m_owner->waterCrop(m_land);
        QTimer::singleShot(0, this, [this]()
        {
            rebuildButtons();
        });
        close();
    }
}

void ActionDialog::onFertilize()
{
    if (m_owner && m_land)
    {
        m_owner->fertilizeCrop(m_land);
        QTimer::singleShot(0, this, [this]()
        {
            rebuildButtons();
        });
        close();
    }
}

void ActionDialog::onRemove()
{
    if (m_owner && m_land)
    {
        m_owner->removeCrop(m_land);
        close();
    }
}

void ActionDialog::onSell()
{
    if (m_owner && m_land)
    {
        m_owner->sellCrop(m_land);
        close();
    }
}
