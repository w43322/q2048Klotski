#ifndef WINDOW2048_H
#define WINDOW2048_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>
#include <QTime>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVector>
#include <QTimer>
#include <QMessageBox>

#include "2048.h"

namespace Ui {
class Window2048;
}

class Window2048 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window2048(QWidget *parent = nullptr, uint8_t wid = 4, uint8_t hei = 4);
    explicit Window2048(QWidget *parent = nullptr, const QString& str = "");
    ~Window2048();
    static uint8_t AskFor(const QString& Title,
                           const QString& Label,
                           const QStringList& Options,
                           QWidget* parent);

protected:
    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private slots:
    void on_pushButtonHint_clicked();
    void on_pushButtonAuto_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonNewGame_clicked();
    void on_checkBox_stateChanged(int arg1);
    void SetTime();

private:
    Ui::Window2048 *ui;
    void Save();
    void Exit();
    void ProcessPress(uint8_t);
    void InitSetup();
    void KeepAspectRatio();
    void InitGridOfLabels();
    void DrawGridOfLabels();
    void SetBorderLenAndTileLen();
    void Sleep(uint32_t);
    void SetScore();
    void SetHint();
    void Undo();
    Grid2048 AnimateParallelMove(uint8_t);
    void AnimatePop(const Grid2048& oldGrid);
    map<PairOfInt8, QLabel> GridOfLabels;
    deque<Game2048> previousGames;
    uint16_t BorderLen;
    uint16_t TileLen;
    Game2048 GAME;
    uint8_t nowGame;
    bool hint = 0;
    bool aut0 = 0;
    bool anim = 1;
    bool inAnimation = 0;
    uint8_t bestMove = 0;
    uint32_t gameTimeDelta = 0;
    QTime gameTime;
    QTimer timer;
    const int AnimationDuration = 300;
    const float BorderToTileRatio = 14.0f / 107;
    const float BorderRaduisToTileRatio = 7.0f / 107;
    const QString COMMON_STYLE = "qproperty-alignment: AlignCenter; font-family: Menlo; font-weight: bold;";
    const QStringList LABEL_STYLES =
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
};

#endif // WINDOW2048_H
