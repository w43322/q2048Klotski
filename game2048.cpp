#include "game2048.h"
#include "ui_game2048.h"

Game2048::Game2048(QWidget *parent) :
    QMainWindow(parent)
    , GAME(4, 4)
    , ui(new Ui::Game2048)
{
    ui->setupUi(this);
    SetScore();
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
    // HERE
    qDebug() << QString::number(TileLen * FontSizeToTileRatio);
    uint16_t x, y = BorderLen;
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i, y += TileLen + BorderLen)
    {
        x = BorderLen;
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j, x += TileLen + BorderLen)
        {
            PairOfInt8 loc = make_pair(i, j);
            uint8_t val = GAME.GetTileVal(loc);
            GridOfLabels[loc].setText(QString::number(val));
            GridOfLabels[loc].setStyleSheet(COMMON_STYLE + LABEL_STYLES[val] + "font-size:" + QString::number(TileLen * FontSizeToTileRatio) + "px;");
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
    DrawGridOfLabels();
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
