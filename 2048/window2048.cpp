#include "mainwindow/mainwindow.h"

#include "window2048.h"
#include "ui_window2048.h"

Window2048::Window2048(QWidget *parent, uint8_t wid, uint8_t hei) :
    QMainWindow(parent)
    , ui(new Ui::Window2048)
    , GAME(wid, hei)
{
    ui->setupUi(this);
    gameTimeDelta = 0;
    InitSetup();
}

Window2048::Window2048(QWidget *parent, const QString& str) :
    QMainWindow(parent)
    , ui(new Ui::Window2048)
    , GAME(str, ((MainWindow*)parent)->FindElement("Score2048").toUInt())
{
    ui->setupUi(this);
    gameTimeDelta = ((MainWindow*)parent)->FindElement("GameTime2048").toUInt();
    InitSetup();
}

Window2048::~Window2048()
{
    on_pushButtonStop_clicked();
    delete ui;
}

void Window2048::InitSetup()
{
    nowGame = 0;
    previousGames.clear();
    previousGames.push_back(GAME);
    connect(&timer, SIGNAL(timeout()), this, SLOT(SetTime()));
    gameTime.start();
    timer.start(1000);
    SetTime();
    hint = 0;
    ui->labelHint->hide();
    ui->pushButtonAuto->hide();
    ui->pushButtonStop->hide();
    aut0 = 0;
    SetScore();
    SetHint();
    KeepAspectRatio();
    InitGridOfLabels();
    DrawGridOfLabels();
    if (GAME.GetWidth() != 4 || GAME.GetHeight() != 4)
        ui->pushButtonHint->hide();
    else
        ui->pushButtonHint->show();
}

void Window2048::Undo()
{
    if (nowGame)
    {
        --nowGame;
        GAME = previousGames[nowGame];
        SetScore();
        DrawGridOfLabels();
    }
}

void Window2048::on_pushButtonNewGame_clicked()
{
    auto wid = AskFor("Width", "Please Select Width:", {"4","3","5","6","7","8","9","10"}, this);
    if (!wid)
        return;
    auto hei = AskFor("Height", "Please Select Height:", {"4","3","5","6","7","8","9","10"}, this);
    if (!hei)
        return;
    GAME = Game2048(wid, hei);
    gameTimeDelta = 0;
    InitSetup();
}

void Window2048::on_pushButtonHint_clicked()
{
    hint = !hint;
    if (hint == 0)
    {
        ui->labelHint->hide();
        ui->pushButtonAuto->hide();
        ui->pushButtonStop->hide();
        return;
    }
    else
    {
        SetHint();
        ui->labelHint->show();
        ui->pushButtonAuto->show();
        ui->pushButtonStop->show();
    }
}

void Window2048::on_pushButtonStop_clicked()
{
    aut0 = false;
    ui->pushButtonNewGame->setEnabled(true);
    ui->pushButtonAuto->setEnabled(true);
    ui->pushButtonHint->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void Window2048::on_pushButtonAuto_clicked()
{
    aut0 = true;
    ui->pushButtonNewGame->setEnabled(false);
    ui->pushButtonAuto->setEnabled(false);
    ui->pushButtonHint->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    while (aut0)
    {
        Sleep(!anim);
        uint8_t mov = bestMove;
        if (!mov)
            break;
        ProcessPress(mov);
    }
}

void Window2048::InitGridOfLabels()
{
    GridOfLabels.clear();
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            GridOfLabels[loc].setParent(ui->widgetGrid);
            GridOfLabels[loc].show();
        }
}

void Window2048::DrawGridOfLabels()
{
    uint16_t x, y = BorderLen;
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i, y += TileLen + BorderLen)
    {
        x = BorderLen;
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j, x += TileLen + BorderLen)
        {
            PairOfInt8 loc = make_pair(i, j);
            uint8_t val = GAME.GetTileVal(loc);
            float FontSizeToTileRatio;
            switch (val)
            {
            case 12:case 13:case 14:case 15:
                FontSizeToTileRatio = 30.0f / 107;
                break;
            case 10:case 11:
                FontSizeToTileRatio = 35.0f / 107;
                break;
            case 7:case 8:case 9:
                FontSizeToTileRatio = 45.0f / 107;
                break;
            default:
                FontSizeToTileRatio = 55.0f / 107;
            }
            QString STYLE = COMMON_STYLE + LABEL_STYLES[val] + "font-size:" +
                    QString::number((int)(TileLen * FontSizeToTileRatio)) + "px;"
                    + "border-radius:" + QString::number((int)(TileLen * BorderRaduisToTileRatio)) + "px;";
            QString DISP = val == 0 ? "" : QString::number(1 << val);
            GridOfLabels[loc].setText(DISP);
            GridOfLabels[loc].setStyleSheet(STYLE);
            GridOfLabels[loc].setGeometry(x, y, TileLen, TileLen);
        }
    }
}

Grid2048 Window2048::AnimateParallelMove(uint8_t direction)
{
    Grid2048 res(GAME.GetWidth(), GAME.GetHeight());
    //qDebug() << "AnimateParallelMove";
    map<PairOfInt8, PairOfInt8> locationChange;
    switch (direction)
    {
    case 'w':
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            uint8_t firstEmptyI = -1;
            uint8_t lastTileVal = 0;
            for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
            {
                PairOfInt8 oldLoc = make_pair(i, j);
                int nowTileVal = GAME.GetTileVal(oldLoc);
                if (nowTileVal == 0) // an empty tile
                    continue;
                if (nowTileVal != lastTileVal)
                    ++firstEmptyI; // non-mergeable
                else
                    nowTileVal = 0; // mark as non-mergeable
                PairOfInt8 newLoc = make_pair(firstEmptyI, j);
                if (oldLoc != newLoc)
                    locationChange[oldLoc] = newLoc;
                res.SetTileVal(newLoc, nowTileVal);
                //qDebug() << "oldLoc:" << oldLoc.first << oldLoc.second << "newLoc:" << newLoc.first << newLoc.second;
                lastTileVal = nowTileVal;
            }
        }
        break;
    case 'a':
        for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        {
            uint8_t firstEmptyJ = -1;
            uint8_t lastTileVal = 0;
            for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
            {
                PairOfInt8 oldLoc = make_pair(i, j);
                int nowTileVal = GAME.GetTileVal(oldLoc);
                if (nowTileVal == 0) // an empty tile
                    continue;
                if (nowTileVal != lastTileVal)
                    ++firstEmptyJ; // non-mergeable
                else
                    nowTileVal = 0; // mark as non-mergeable
                PairOfInt8 newLoc = make_pair(i, firstEmptyJ);
                if (oldLoc != newLoc)
                    locationChange[oldLoc] = newLoc;
                res.SetTileVal(newLoc, nowTileVal);
                //qDebug() << "oldLoc:" << oldLoc.first << oldLoc.second << "newLoc:" << newLoc.first << newLoc.second;
                lastTileVal = nowTileVal;
            }
        }
        break;
    case 's':
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            uint8_t firstEmptyI = GAME.GetHeight();
            uint8_t lastTileVal = 0;
            for (uint8_t i = GAME.GetHeight() - 1; i != 255; --i)
            {
                PairOfInt8 oldLoc = make_pair(i, j);
                int nowTileVal = GAME.GetTileVal(oldLoc);
                if (nowTileVal == 0) // an empty tile
                    continue;
                if (nowTileVal != lastTileVal)
                    --firstEmptyI; // non-mergeable
                else
                    nowTileVal = 0; // mark as non-mergeable
                PairOfInt8 newLoc = make_pair(firstEmptyI, j);
                if (oldLoc != newLoc)
                    locationChange[oldLoc] = newLoc;
                res.SetTileVal(newLoc, nowTileVal);
                //qDebug() << "oldLoc:" << oldLoc.first << oldLoc.second << "newLoc:" << newLoc.first << newLoc.second;
                lastTileVal = nowTileVal;
            }
        }
        break;
    case 'd':
        for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
        {
            uint8_t firstEmptyJ = GAME.GetWidth();
            uint8_t lastTileVal = 0;
            for (uint8_t j = GAME.GetWidth() - 1; j!= 255; --j)
            {
                PairOfInt8 oldLoc = make_pair(i, j);
                int nowTileVal = GAME.GetTileVal(oldLoc);
                if (nowTileVal == 0) // an empty tile
                    continue;
                if (nowTileVal != lastTileVal)
                    --firstEmptyJ; // non-mergeable
                else
                    nowTileVal = 0; // mark as non-mergeable
                PairOfInt8 newLoc = make_pair(i, firstEmptyJ);
                if (oldLoc != newLoc)
                    locationChange[oldLoc] = newLoc;
                res.SetTileVal(newLoc, nowTileVal);
                //qDebug() << "oldLoc:" << oldLoc.first << oldLoc.second << "newLoc:" << newLoc.first << newLoc.second;
                lastTileVal = nowTileVal;
            }
        }
        break;
    }
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QVector<QLabel*> placeholderArray;
    for (auto&& pairLocations : locationChange)
    {
        PairOfInt8 oldLoc = pairLocations.first, newLoc = pairLocations.second;
        uint8_t newY = newLoc.first, newX = newLoc.second;
        auto&& pTile = &GridOfLabels[oldLoc];
        pTile->raise();

        // Create Animation
        QPropertyAnimation* animation = new QPropertyAnimation(pTile, "geometry");
        animation->setDuration(AnimationDuration*0.4);
        animation->setEndValue(
                    QRect(BorderLen * (newX + 1) + TileLen * newX,
                          BorderLen * (newY + 1) + TileLen * newY,
                          pTile->width(),
                          pTile->height()));
        group->addAnimation(animation);

        // Create Placeholder QLabel
        QLabel* pLabel = new QLabel(ui->widgetGrid);
        QString STYLE = COMMON_STYLE + LABEL_STYLES[0] +
                "border-radius:" + QString::number((int)(TileLen * BorderRaduisToTileRatio)) + "px;";
        pLabel->setStyleSheet(STYLE);
        pLabel->setGeometry(pTile->geometry());
        pLabel->lower();
        pLabel->show();
        placeholderArray.push_back(pLabel);
    }
    group->start(QAbstractAnimation::DeleteWhenStopped);
    Sleep(AnimationDuration*0.5);
    for (auto&& pLabel : placeholderArray)
        delete pLabel;
    return res;
}

void Window2048::AnimatePop(const Grid2048& oldGrid)
{
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    for (uint8_t i = 0; i < GAME.GetHeight(); ++i)
    {
        for (uint8_t j = 0; j < GAME.GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            if (oldGrid.GetTileVal(loc) != GAME.GetTileVal(loc))
            {
                auto&& pTile = &GridOfLabels[loc];
                QPropertyAnimation* animation = new QPropertyAnimation(pTile, "geometry");
                animation->setDuration(AnimationDuration*0.2);
                animation->setStartValue(
                            QRect(pTile->x(),
                                  pTile->y(),
                                  pTile->width(),
                                  pTile->height()));
                animation->setEndValue(
                            QRect(pTile->x()-0.5*BorderToTileRatio*pTile->width(),
                                  pTile->y()-0.5*BorderToTileRatio*pTile->height(),
                                  (BorderToTileRatio + 1.0)*pTile->width(),
                                  (BorderToTileRatio + 1.0)*pTile->height()));
                group->addAnimation(animation);
            }
        }
    }
    group->start(QAbstractAnimation::KeepWhenStopped);
    Sleep(AnimationDuration*0.25);
    group->setDirection(QAbstractAnimation::Backward);
    group->start(QAbstractAnimation::DeleteWhenStopped);
    Sleep(AnimationDuration*0.25);
}

void Window2048::ProcessPress(uint8_t direction)
{
    if (anim)
    {
        ui->checkBox->setEnabled(aut0);
        inAnimation = true;
        Grid2048 oldGrid = AnimateParallelMove(direction);
        GAME.Step(direction);
        DrawGridOfLabels();
        AnimatePop(oldGrid);
        inAnimation = false;
        ui->checkBox->setEnabled(true);
    }
    else
    {
        GAME.Step(direction);
        DrawGridOfLabels();
    }
    SetScore();
    if (hint)
        SetHint();

    // Update undo Queue
    if (!(GAME == previousGames.back()))
    {
        if (nowGame == 255)
            previousGames.pop_front();
        else
            ++nowGame;
        if (nowGame < previousGames.size())
            previousGames[nowGame] = GAME;
        else
            previousGames.push_back(GAME);
    }

    // Game Over Logic
    if (GAME.GameOver())
    {
        timer.stop();
        if (aut0)
            on_pushButtonStop_clicked();
        auto ans = QMessageBox::question(
                    this,
                    "Game Over!",
                    "Your game is over :(\nDo you want to restart?",
                    QMessageBox::No, QMessageBox::Yes);
        if(ans == QMessageBox::Yes)
            on_pushButtonNewGame_clicked();
        else
        {
            Save();
            Exit();
        }
    }
}

void Window2048::Save()
{
    ((MainWindow*)parent())->UpdateElement("Score2048", QString::number(GAME.GetScore()));
    ((MainWindow*)parent())->UpdateElement("GameTime2048", QString::number(gameTime.elapsed() + gameTimeDelta));
    ((MainWindow*)parent())->UpdateElement("Grid2048", GAME.ToQString());
}

void Window2048::Exit()
{
    ((MainWindow*)parent())->show();
    hide();
    delete this;
    return;
}

void Window2048::keyPressEvent(QKeyEvent *event)
{
    if (aut0 || inAnimation)
        return;
    Game2048 lastGameState = GAME;
    switch (event->key())
    {
    case Qt::Key_W:case Qt::Key_Up:
        ProcessPress('w');
        break;
    case Qt::Key_A:case Qt::Key_Left:
        ProcessPress('a');
        break;
    case Qt::Key_S:case Qt::Key_Down:
        ProcessPress('s');
        break;
    case Qt::Key_D:case Qt::Key_Right:
        ProcessPress('d');
        break;
    case Qt::Key_Z:
        Undo();
        break;
    case Qt::Key_Escape:
        Save();
        Exit();
        break;
    default:
        return;
    }
}

void Window2048::closeEvent(QCloseEvent *e)
{
    if (aut0)
        on_pushButtonStop_clicked();
    auto ans = QMessageBox::question(
                this,
                "Quit",
                "Are you sure to quit this application?",
                QMessageBox::Yes, QMessageBox::No);
    if(ans == QMessageBox::Yes)
    {
        Save();
        e->accept();
    }
    else
        e->ignore();
}

void Window2048::SetHint()
{
    QString STYLE1 =
    "style = '"
    "font-size: 13px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 25px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">HINT</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    switch (GAME.GetBestMove())
    {
    case 'w': // up
        TEXT += "UP";
        bestMove = 'w';
        break;
    case 's': // down
        TEXT += "DOWN";
        bestMove = 's';
        break;
    case 'a': // left
        TEXT += "LEFT";
        bestMove = 'a';
        break;
    case 'd': // right
        TEXT += "RIGHT";
        bestMove = 'd';
        break;
    default:
        TEXT += "N/A";
    }
    TEXT +="</font>";
    ui->labelHint->setText(TEXT);
}

void Window2048::SetScore()
{
    QString STYLE1 =
    "style = '"
    "font-size: 13px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 25px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">SCORE</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += QString::number(GAME.GetScore()) + "</font>";
    ui->labelScore->setText(TEXT);
}

void Window2048::SetTime()
{
    QString STYLE1 =
    "style = '"
    "font-size: 13px;"
    "font-weight: bold;"
    "color: #eee4da;'";
    QString STYLE2 =
    "style = '"
    "font-size: 25px;"
    "font-weight: bold;"
    "color: white;'";
    QString TEXT = "<font " + STYLE1 + ">TIME</font><br/>";
    TEXT += "<font " + STYLE2 + ">";
    TEXT += QDateTime::fromMSecsSinceEpoch(gameTime.elapsed() + gameTimeDelta, Qt::UTC).toString("mm:ss") + "</font>";
    ui->labelTime->setText(TEXT);
}

void Window2048::KeepAspectRatio()
{
    uint8_t hei = GAME.GetHeight(), wid = GAME.GetWidth();
    uint8_t uni;
    if(ui->widgetGridFrame->width() * hei > ui->widgetGridFrame->height() * wid)
        uni = ui->widgetGridFrame->height() / hei;
    else
        uni = ui->widgetGridFrame->width() / wid;
    ui->widgetGrid->setFixedHeight(uni * hei);
    ui->widgetGrid->setFixedWidth(uni * wid);
    SetBorderLenAndTileLen();
    DrawGridOfLabels();
}

void Window2048::SetBorderLenAndTileLen()
{
    uint8_t wid = GAME.GetWidth();
    TileLen = ui->widgetGrid->width() / (BorderToTileRatio + BorderToTileRatio * wid + wid);
    BorderLen = TileLen * BorderToTileRatio;
}

void Window2048::resizeEvent(QResizeEvent *event)
{
    event->accept();
    KeepAspectRatio();
}

void Window2048::Sleep(uint32_t msec)
{
    QTime dieTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

uint8_t Window2048::AskFor(const QString& Title,
                       const QString& Label,
                       const QStringList& Options,
                       QWidget* parent)
{
    bool isok;
    QString STR = QInputDialog::getItem(parent,
                                        Title,
                                        Label,
                                        Options,
                                        0,
                                        false,
                                        &isok);
    return isok ? STR.toUInt() : 0;
}

void Window2048::on_checkBox_stateChanged(int arg1)
{
    anim = arg1;
}

