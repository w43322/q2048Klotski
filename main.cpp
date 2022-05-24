#include "mainwindow/mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QFontDatabase::addApplicationFont(":/fonts/menlo.ttf");
    QFontDatabase::addApplicationFont(":/fonts/monaco.ttf");
    QApplication a(argc, argv);
    //a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.show();
    return a.exec();
}
