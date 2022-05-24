#include "mainwindow/mainwindow.h"

#include "windowklotski.h"
#include "ui_windowklotski.h"

WindowKlotski::WindowKlotski(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowKlotski)
    ,GAME(AskFor("Game", "Please Select Game:", {"横刀立马", "指挥若定", "数字华容道"}, "横刀立马", parent))
{
    ui->setupUi(this);
    InitSetup();
}

void WindowKlotski::on_pushButton_clicked()
{
    GAME = GameKlotski(AskFor("Game", "Please Select Game:", {"横刀立马", "指挥若定", "数字华容道"}, "横刀立马", this));
    InitSetup();
}


void WindowKlotski::InitSetup()
{
    GAME.Update();
    SelectedLoc = GAME.NextAdjRight(make_pair(0, 0));
    SetStep();
    KeepAspectRatio();
    InitGridOfLabels();
    DrawGridOfLabels();
}

WindowKlotski::~WindowKlotski()
{
    delete ui;
}

QString WindowKlotski::AskFor(const QString& Title,
                       const QString& Label,
                       const QStringList& Options,
                       const QString& defVal,
                       QWidget* parent)
{
    bool isok;
    QString STR = QInputDialog::getItem(parent,
                                        Title,
                                        Label,
                                        Options,
                                        0,
                                        false,
                                        &isok);
    return isok ? STR : defVal;
}

void WindowKlotski::resizeEvent(QResizeEvent *event)
{
    event->accept();
    KeepAspectRatio();
}

void WindowKlotski::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
            SelectedLoc = GAME.NextAdjUp(SelectedLoc);
            break;
        case Qt::Key_Left:
            SelectedLoc = GAME.NextAdjLeft(SelectedLoc);
            break;
        case Qt::Key_Down:
            SelectedLoc = GAME.NextAdjDown(SelectedLoc);
            break;
        case Qt::Key_Right:
            SelectedLoc = GAME.NextAdjRight(SelectedLoc);
            break;
        case Qt::Key_W:
            SelectedLoc = GAME.Move(SelectedLoc,'w');
            break;
        case Qt::Key_A:
            SelectedLoc = GAME.Move(SelectedLoc,'a');
            break;
        case Qt::Key_S:
            SelectedLoc = GAME.Move(SelectedLoc,'s');
            break;
        case Qt::Key_D:
            SelectedLoc = GAME.Move(SelectedLoc,'d');
            break;
        case Qt::Key_Escape:
            ((MainWindow*)parent())->show();
            hide();
            delete this;
            return;
    default:
            //qDebug() << event->key() << endl;
            break;
    }
    SetStep();
    DrawGridOfLabels();
}

void WindowKlotski::InitGridOfLabels()
{
    GridOfLabels.clear();
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            GridOfLabels[loc].setParent(ui->widgetGrid);
        }
}

void WindowKlotski::DrawGridOfLabels()
{
    uint16_t x, y = BorderLen;
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i, y += TileLen + BorderLen)
    {
        x = BorderLen;
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j, x += TileLen + BorderLen)
        {
            PairOfInt8 loc = make_pair(i, j);
            uint8_t wid = GAME.GetTileWidth(loc);
            uint8_t hei = GAME.GetTileHeight(loc);
            if (wid == 0 || hei == 0)
            {
                GridOfLabels[loc].hide();
                continue;
            }
            uint8_t val = GAME.GetTileVal(loc);
            QString STYLE = COMMON_STYLE + LABEL_STYLES[val] + "font-size:" +
                    QString::number((int)(TileLen * FontSizeToTileRatio)) + "px;"
                    + "border-radius:" + QString::number((int)(TileLen * BorderRaduisToTileRatio)) + "px;";
            QString DISP = QString::number(val);
            if (loc == SelectedLoc)
                STYLE += "border-radius:0px;border-style:ridge;border-width:"
                        + QString::number((int)(0.618 * BorderLen))
                        + "px;border-color:#8f7a66";
            GridOfLabels[loc].setText(DISP);
            GridOfLabels[loc].setStyleSheet(STYLE);
            GridOfLabels[loc].setGeometry(x, y, TileLen * wid + BorderLen * (wid - 1), TileLen * hei + BorderLen * (hei - 1));
            GridOfLabels[loc].show();
        }
    }
}

void WindowKlotski::KeepAspectRatio()
{
    uint8_t hei = GAME.GetHeight(), wid = GAME.GetWidth();
    uint8_t uni;
    if(ui->widgetGridFrame->width() * hei > ui->widgetGridFrame->height() * wid)
        uni = ui->widgetGridFrame->height() / hei;
    else
        uni = ui->widgetGridFrame->width() / wid;
    ui->widgetGrid->setFixedHeight(uni * hei);
    ui->widgetGrid->setFixedWidth(uni * wid);
    SetBorderLenAndTileLen();
    DrawGridOfLabels();
}

void WindowKlotski::SetBorderLenAndTileLen()
{
    uint8_t wid = GAME.GetWidth();
    TileLen = ui->widgetGrid->width() / (BorderToTileRatio + BorderToTileRatio * wid + wid);
    BorderLen = TileLen * BorderToTileRatio;
}

void WindowKlotski::SetStep()
{
    QString STYLE1 =
    "style = '"
    "font-size: 13px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 25px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">STEP</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += QString::number(GAME.GetStep()) + "</font>";
    ui->labelScore->setText(TEXT);
}
