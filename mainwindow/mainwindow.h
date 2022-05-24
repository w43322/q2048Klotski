#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>

#include "2048/window2048.h"
#include "klotski/windowklotski.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonRun2048_clicked();

    void on_pushButtonRunKlotski_clicked();

private:
    Ui::MainWindow *ui;
    Window2048 *uiWindow2048;
    WindowKlotski *uiWindowKlotski;
    friend class Window2048;
    friend class WindowKlotski;
};
#endif // MAINWINDOW_H
