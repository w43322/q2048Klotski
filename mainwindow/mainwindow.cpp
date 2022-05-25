#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":/fonts/Monaco.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Menlo-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Menlo-Regular.ttf");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRun2048_clicked()
{
    auto wid = Window2048::AskFor("Width", "Please Select Width:", {"4","3","5","6","7","8","9","10"}, this);
    if (!wid)
        return;
    auto hei = Window2048::AskFor("Height", "Please Select Height:", {"4","3","5","6","7","8","9","10"}, this);
    if (!hei)
        return;
    uiWindow2048 = new Window2048(this, wid, hei);
    uiWindow2048->show();
    hide();
}

void MainWindow::on_pushButtonRunKlotski_clicked()
{
    uiWindowKlotski = new WindowKlotski(this);
    uiWindowKlotski->show();
    hide();
}

