#ifndef WINDOWKLOTSKI_H
#define WINDOWKLOTSKI_H

#include <QMainWindow>
#include <QInputDialog>
#include <QLabel>

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

protected:
    virtual void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;

private:
    void InitGridOfLabels();
    void DrawGridOfLabels();
    void KeepAspectRatio();
    void SetBorderLenAndTileLen();
    QString AskFor(const QString& Title,
                           const QString& Label,
                           const QStringList& Options,
                           const QString& defVal,
                           QWidget* parent);
    Ui::WindowKlotski *ui;
    map<PairOfInt8, QLabel> GridOfLabels;
    GameKlotski GAME;
    uint16_t BorderLen;
    uint16_t TileLen;
    const float BorderToTileRatio = 14.0f / 107;
    const float BorderRaduisToTileRatio = 7.0f / 107;
};

#endif // WINDOWKLOTSKI_H
