#include "windowklotski.h"
#include "ui_windowklotski.h"

WindowKlotski::WindowKlotski(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowKlotski)
{
    ui->setupUi(this);
}

WindowKlotski::~WindowKlotski()
{
    delete ui;
}
