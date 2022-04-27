#ifndef WINDOWKLOTSKI_H
#define WINDOWKLOTSKI_H

#include <QMainWindow>

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
    Ui::WindowKlotski *ui;
};

#endif // WINDOWKLOTSKI_H
