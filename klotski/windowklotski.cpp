#include "windowklotski.h"
#include "ui_windowklotski.h"

WindowKlotski::WindowKlotski(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowKlotski)
    //,GAME(AskFor("Width", "Please Select Width:", {"横刀立马", "数字华容道"}, "横刀立马", parent))
{
    ui->setupUi(this);
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
