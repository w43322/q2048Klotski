#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , GAME(4, 4)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_W:
            GAME.Step('w');
            break;
        case Qt::Key_A:
            GAME.Step('a');
            break;
        case Qt::Key_S:
            GAME.Step('s');
            break;
        case Qt::Key_D:
            GAME.Step('d');
            break;
    default:
            //qDebug() << event->key() << endl;
            break;
    }
}
