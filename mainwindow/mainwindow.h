#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QWidget>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDialog>
#include <QString>

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
    void LogIn();
    void SetWelcomeLabel();

private slots:
    void on_pushButtonRun2048_clicked();

    void on_pushButtonRunKlotski_clicked();

    void on_loginBtn_clicked();

    void on_logoutBtn_clicked();

private:
    Ui::MainWindow *ui;
    Window2048 *uiWindow2048;
    WindowKlotski *uiWindowKlotski;

    QDomDocument mydoc;
    QString userName;

    friend class Window2048;
    friend class WindowKlotski;
};
#endif // MAINWINDOW_H
