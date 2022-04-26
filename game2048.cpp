#include "game2048.h"
#include "ui_game2048.h"

Game2048::Game2048(QWidget *parent) :
    QMainWindow(parent)
    , GAME(4, 4)
    , ui(new Ui::Game2048)
{
    ui->setupUi(this);
    SetScore();
    SetHint();
    KeepAspectRatio();
    InitGridOfLabels();
    DrawGridOfLabels();
}

Game2048::~Game2048()
{
    delete ui;
}

void Game2048::InitGridOfLabels()
{
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
            GridOfLabels[make_pair(i, j)].setParent(ui->widgetGrid);
}

void Game2048::DrawGridOfLabels()
{
    uint16_t x, y = BorderLen;
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i, y += TileLen + BorderLen)
    {
        x = BorderLen;
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j, x += TileLen + BorderLen)
        {
            PairOfInt8 loc = make_pair(i, j);
            uint8_t val = GAME.GetTileVal(loc);
            float FontSizeToTileRatio;
            float BorderRaduisToTileRatio = 7.0f / 107;
            switch (val)
            {
            case 12:case 13:case 14:case 15:
                FontSizeToTileRatio = 30.0f / 107;
                break;
            case 10:case 11:
                FontSizeToTileRatio = 35.0f / 107;
                break;
            case 7:case 8:case 9:
                FontSizeToTileRatio = 45.0f / 107;
                break;
            default:
                FontSizeToTileRatio = 55.0f / 107;
            }
            QString STYLE = COMMON_STYLE + LABEL_STYLES[val] + "font-size:" +
                    QString::number((int)(TileLen * FontSizeToTileRatio)) + "px;"
                    + "border-radius:" + QString::number((int)(TileLen * BorderRaduisToTileRatio)) + "px;";
            QString DISP = val == 0 ? "" : QString::number(1 << val);
            GridOfLabels[loc].setText(DISP);
            GridOfLabels[loc].setStyleSheet(STYLE);
            GridOfLabels[loc].setGeometry(x, y, TileLen, TileLen);
        }
    }
}

void Game2048::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_W:case Qt::Key_Up:
            GAME.Step('w');
            break;
        case Qt::Key_A:case Qt::Key_Left:
            GAME.Step('a');
            break;
        case Qt::Key_S:case Qt::Key_Down:
            GAME.Step('s');
            break;
        case Qt::Key_D:case Qt::Key_Right:
            GAME.Step('d');
            break;
    default:
            //qDebug() << event->key() << endl;
            break;
    }
    SetScore();
    SetHint();
    DrawGridOfLabels();
}

void Game2048::SetHint()
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
    QString TEXT = "<font " + STYLE1 + ">HINT</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    switch (GAME.GetBestMove())
    {
    case 'w': // up
        TEXT += "UP";
        break;
    case 's': // down
        TEXT += "DOWN";
        break;
    case 'a': // left
        TEXT += "LEFT";
        break;
    case 'd': // right
        TEXT += "RIGHT";
        break;
    default:
        TEXT += "N/A";
    }
    TEXT +="</font>";
    ui->labelHint->setText(TEXT);
}

void Game2048::SetScore()
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
    QString TEXT = "<font " + STYLE1 + ">SCORE</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += QString::number(GAME.GetScore()) + "</font>";
    ui->labelScore->setText(TEXT);
}

void Game2048::KeepAspectRatio()
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

void Game2048::SetBorderLenAndTileLen()
{
    uint8_t wid = GAME.GetWidth();
    TileLen = ui->widgetGrid->width() / (BorderToTileRatio + BorderToTileRatio * wid + wid);
    BorderLen = TileLen * BorderToTileRatio;
}

void Game2048::resizeEvent(QResizeEvent *event)
{
    event->accept();
    KeepAspectRatio();
    DrawGridOfLabels();
}
