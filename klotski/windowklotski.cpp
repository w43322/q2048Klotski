#include "mainwindow/mainwindow.h"

#include "windowklotski.h"
#include "ui_windowklotski.h"

WindowKlotski::WindowKlotski(QWidget *parent, const QString& ans) :
    QMainWindow(parent),
    ui(new Ui::WindowKlotski)
    ,GAME(ans)
{
    ui->setupUi(this);
    InitSetup();
}

void WindowKlotski::on_pushButton_clicked()
{
    auto ans = AskFor("Game", "Please Select Game:", GameKlotski::GameSetStringList, this);
    if (ans == "")
        return;
    GAME = GameKlotski(ans);
    InitSetup();
}

void WindowKlotski::InitSetup()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(SetTime()));
    gameTime.start();
    timer.start(1000);
    SetTime();
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
    return isok ? STR : "";
}

void WindowKlotski::AfterMove()
{
    if (!GAME.GameOver())
        return;
    auto ans = QMessageBox::question(
                this,
                "Game Over!",
                "Your game is over :(\nDo you want to restart?",
                QMessageBox::No, QMessageBox::Yes);
    if(ans == QMessageBox::Yes)
        on_pushButton_clicked();
    else
    {
        ((MainWindow*)parent())->show();
        hide();
        delete this;
        return;
    }
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
        DrawGridOfLabels();
        break;
    case Qt::Key_Left:
        SelectedLoc = GAME.NextAdjLeft(SelectedLoc);
        DrawGridOfLabels();
        break;
    case Qt::Key_Down:
        SelectedLoc = GAME.NextAdjDown(SelectedLoc);
        DrawGridOfLabels();
        break;
    case Qt::Key_Right:
        SelectedLoc = GAME.NextAdjRight(SelectedLoc);
        DrawGridOfLabels();
        break;
    case Qt::Key_W:
        SelectedLoc = GAME.Move(SelectedLoc,'w');
        DrawGridOfLabels();
        AfterMove();
        break;
    case Qt::Key_A:
        SelectedLoc = GAME.Move(SelectedLoc,'a');
        DrawGridOfLabels();
        AfterMove();
        break;
    case Qt::Key_S:
        SelectedLoc = GAME.Move(SelectedLoc,'s');
        DrawGridOfLabels();
        AfterMove();
        break;
    case Qt::Key_D:
        SelectedLoc = GAME.Move(SelectedLoc,'d');
        DrawGridOfLabels();
        AfterMove();
        break;
    case Qt::Key_Escape:
        ((MainWindow*)parent())->show();
        hide();
        delete this;
        return;
    default:
        return;
    }
}

void WindowKlotski::closeEvent(QCloseEvent *e)
{
    auto ans = QMessageBox::question(
                this,
                "Quit",
                "Are you sure to quit this application?",
                QMessageBox::Yes, QMessageBox::No);
    if(ans == QMessageBox::Yes)
        e->accept();
    else
        e->ignore();
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

void WindowKlotski::SetTime()
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
    QString TEXT = "<font " + STYLE1 + ">TIME</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += QDateTime::fromMSecsSinceEpoch(gameTime.elapsed(), Qt::UTC).toString("mm:ss") + "</font>";
    ui->labelTime->setText(TEXT);
}
