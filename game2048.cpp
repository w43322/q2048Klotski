#include "game2048.h"
#include "ui_game2048.h"

Game2048::Game2048(QWidget *parent) :
    QMainWindow(parent)
    , GAME(4, 4)
    , ui(new Ui::Game2048)
{
    ui->setupUi(this);
}

Game2048::~Game2048()
{
    delete ui;
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
}
