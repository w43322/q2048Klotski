#ifndef WINDOWKLOTSKI_H
#define WINDOWKLOTSKI_H

#include <QMainWindow>
#include <QInputDialog>

#include "klotski.h"

namespace Ui {
class WindowKlotski;
}

class WindowKlotski : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowKlotski(QWidget *parent = nullptr);
    ~WindowKlotski();

private:
    QString AskFor(const QString& Title,
                           const QString& Label,
                           const QStringList& Options,
                           const QString& defVal,
                           QWidget* parent);
    Ui::WindowKlotski *ui;
    //GameKlotski GAME;
};

#endif // WINDOWKLOTSKI_H
