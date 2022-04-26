#ifndef GAME2048_H
#define GAME2048_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>

#include "2048.h"

const QString COMMON_STYLE = "qproperty-alignment: AlignCenter; font-weight: bold;";
const QString LABEL_STYLES[] =
{
    "background: #cdc1b4; color: #776e65;", // 0,  1
    "background: #eee4da; color: #776e65;", // 1,  2
    "background: #eee1c9; color: #776e65;", // 2,  4
    "background: #f3b27a; color: #f9f6f2;", // 3,  8
    "background: #f69664; color: #f9f6f2;", // 4,  16
    "background: #f77c5f; color: #f9f6f2;", // 5,  32
    "background: #f75f3b; color: #f9f6f2;", // 6,  64
    "background: #edd073; color: #f9f6f2;", // 7,  128
    "background: #edcc62; color: #f9f6f2;", // 8,  256
    "background: #edc950; color: #f9f6f2;", // 9,  512
    "background: #edc53f; color: #f9f6f2;", // 10, 1024
    "background: #edc22e; color: #f9f6f2;", // 11, 2048
    "background: #3c3a33; color: #f9f6f2;", // 12, 4096
    "background: #3c3a33; color: #f9f6f2;", // 13, 8192
    "background: #3c3a33; color: #f9f6f2;", // 14, 16384
    "background: #3c3a33; color: #f9f6f2;"  // 15, 32768
};

namespace Ui {
class Game2048;
}

class Game2048 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game2048(QWidget *parent = nullptr);
    ~Game2048();

    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
    Game GAME;
    void SetScore();
    void SetHint();

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


private:
    Ui::Game2048 *ui;
    void KeepAspectRatio();
    void InitGridOfLabels();
    void DrawGridOfLabels();
    void SetBorderLenAndTileLen();
    map<PairOfInt8, QLabel> GridOfLabels;
    float BorderToTileRatio = 14.0f / 107;
    uint16_t BorderLen;
    uint16_t TileLen;
};

#endif // GAME2048_H
