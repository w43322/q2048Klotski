#ifndef GAME2048_H
#define GAME2048_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>

#include "2048.h"

namespace Ui {
class Game2048;
}

class Game2048 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game2048(QWidget *parent = nullptr);
    ~Game2048();

    void keyPressEvent(QKeyEvent *);
    Game GAME;

private:
    Ui::Game2048 *ui;
};

#endif // GAME2048_H
