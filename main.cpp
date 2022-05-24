#include "mainwindow/mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setFont();
    //a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    w.show();
    return a.exec();
}
