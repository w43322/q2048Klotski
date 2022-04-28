#include "mainwindow/mainwindow.h"

#include "windowklotski.h"
#include "ui_windowklotski.h"

WindowKlotski::WindowKlotski(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowKlotski)
    ,GAME(AskFor("Width", "Please Select Width:", {"横刀立马", "数字华容道"}, "横刀立马", parent))
{
    ui->setupUi(this);
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
        /*case Qt::Key_W:case Qt::Key_Up:
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
            break;*/
        case Qt::Key_Escape:
            ((MainWindow*)parent())->show();
            hide();
            delete this;
            return;
    default:
            //qDebug() << event->key() << endl;
            break;
    }
    //SetStep();
    //DrawGridOfLabels();
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
            uint8_t val = GAME.GetTileVal(loc);
            /*QString STYLE = COMMON_STYLE + LABEL_STYLES[val] + "font-size:" +
                    QString::number((int)(TileLen * FontSizeToTileRatio)) + "px;"
                    + "border-radius:" + QString::number((int)(TileLen * BorderRaduisToTileRatio)) + "px;";*/
            QString DISP = val == 0 ? "" : QString::number(1 << val);
            GridOfLabels[loc].setText(DISP);
            /*GridOfLabels[loc].setStyleSheet(STYLE);*/
            GridOfLabels[loc].setGeometry(x, y, TileLen * GAME.GetTileWidth(loc), TileLen * GAME.GetTileHeight(loc));
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
    //DrawGridOfLabels();
}

void WindowKlotski::SetBorderLenAndTileLen()
{
    uint8_t wid = GAME.GetWidth();
    TileLen = ui->widgetGrid->width() / (BorderToTileRatio + BorderToTileRatio * wid + wid);
    BorderLen = TileLen * BorderToTileRatio;
}
