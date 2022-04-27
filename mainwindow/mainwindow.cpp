#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRun2048_clicked()
{
    uiWindow2048 = new Window2048(this);
    uiWindow2048->show();
    hide();
}

void MainWindow::on_pushButtonRunKlotski_clicked()
{
    uiWindowKlotski = new WindowKlotski(this);
    uiWindowKlotski->show();
    hide();
}

