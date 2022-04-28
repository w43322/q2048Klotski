#include "mainwindow/mainwindow.h"

#include "window2048.h"
#include "ui_window2048.h"

Window2048::Window2048(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::Window2048)
    , GAME(AskFor("Width", "Please Select Width:", {"4","5","6","7","8","9","10"}, 4, parent)
         , AskFor("Height", "Please Select Height:", {"4","5","6","7","8","9","10"}, 4, parent))
{
    ui->setupUi(this);
    InitSetup();
}

Window2048::~Window2048()
{
    delete ui;
}

void Window2048::InitSetup()
{
    hint = 0;
    aut0 = 0;
    SetScore();
    SetHint();
    KeepAspectRatio();
    InitGridOfLabels();
    DrawGridOfLabels();
    if (GAME.GetWidth() != 4 || GAME.GetHeight() != 4)
        ui->pushButtonHint->hide();
    else
        ui->pushButtonHint->show();
}

void Window2048::on_pushButtonNewGame_clicked()
{
    GAME = Game2048(AskFor("Width", "Please Select Width:", {"4","5","6","7","8","9","10"}, 4, this)
                    , AskFor("Height", "Please Select Height:", {"4","5","6","7","8","9","10"}, 4, this));
    InitSetup();
}

void Window2048::on_pushButtonHint_clicked()
{
    hint = !hint;
    SetHint();
}

void Window2048::on_pushButtonStop_clicked()
{
    aut0 = false;
    ui->pushButtonNewGame->setEnabled(true);
    ui->pushButtonAuto->setEnabled(true);
    ui->pushButtonHint->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void Window2048::on_pushButtonAuto_clicked()
{
    aut0 = true;
    ui->pushButtonNewGame->setEnabled(false);
    ui->pushButtonAuto->setEnabled(false);
    ui->pushButtonHint->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    while (aut0)
    {
        uint8_t mov = GAME.GetBestMove();
        if (!mov)
            break;
        GAME.Step(mov);
        SetScore();
        SetHint();
        DrawGridOfLabels();
        Sleep(100);
    }
    on_pushButtonStop_clicked();
}

void Window2048::InitGridOfLabels()
{
    GridOfLabels.clear();
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            GridOfLabels[loc].setParent(ui->widgetGrid);
            GridOfLabels[loc].show();
        }
}

void Window2048::DrawGridOfLabels()
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

void Window2048::keyPressEvent(QKeyEvent *event)
{
    if (aut0)
        return;
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
        case Qt::Key_Escape:
            ((MainWindow*)parent())->show();
            hide();
            delete this;
            return;
    default:
            //qDebug() << event->key() << endl;
            break;
    }
    SetScore();
    SetHint();
    DrawGridOfLabels();
}

void Window2048::SetHint()
{
    if (hint == 0)
    {
        ui->labelHint->hide();
        ui->pushButtonAuto->hide();
        ui->pushButtonStop->hide();
        return;
    }
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
    ui->labelHint->show();
    ui->pushButtonAuto->show();
    ui->pushButtonStop->show();
}

void Window2048::SetScore()
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

void Window2048::KeepAspectRatio()
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

void Window2048::SetBorderLenAndTileLen()
{
    uint8_t wid = GAME.GetWidth();
    TileLen = ui->widgetGrid->width() / (BorderToTileRatio + BorderToTileRatio * wid + wid);
    BorderLen = TileLen * BorderToTileRatio;
}

void Window2048::resizeEvent(QResizeEvent *event)
{
    event->accept();
    KeepAspectRatio();
}

void Window2048::Sleep(uint32_t msec)
{
    QTime dieTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

uint8_t Window2048::AskFor(const QString& Title,
                       const QString& Label,
                       const QStringList& Options,
                       uint8_t defVal,
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
    return isok ? STR.toUInt() : defVal;
}


