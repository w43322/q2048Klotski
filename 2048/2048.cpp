#include "2048.h"
bool Grid2048::IsFull()
{
    for (uint8_t i = 0; i < height; ++i)
        for (uint8_t j = 0; j < width; ++j)
            if (data[make_pair(i, j)].GetVal() == 0)
                return false;
    return true;
}
void Grid2048::PutNewTileOnRandomLocation()
{
    if (IsFull())
        return;
    PairOfInt8 location = GetRandomEmptyLocation();
    data[location] = rand() < RAND_MAX * 0.9 ? 1 : 2;
    // printf("\nplacing at [%d][%d]", location.first, location.second);
}
PairOfInt8 Grid2048::GetRandomEmptyLocation()
{
    if (IsFull())
        return make_pair(-1, -1);
    uint8_t x = arc4random() % height;
    uint8_t y = arc4random() % width;
    while (data[make_pair(x, y)].GetVal())
    {
        x = arc4random() % height;
        y = arc4random() % width;
    }
    return make_pair(x, y);
}
uint16_t Grid2048::Merge(uint8_t direction)
{
    uint16_t res = 0;
    // pre-process
    switch (direction)
    {
    case 'w':
        RotateLeft();
        break;
    case 's':
        RotateRight();
        break;
    case 'd':
        HorizontalMirror();
        break;
    }
    // merge (to the left)
    for (uint8_t i = 0; i < height; ++i)
    {
        deque<uint8_t> vec;
        for (uint8_t j = 0; j < width; ++j)
        {
            uint8_t val = data[make_pair(i, j)].GetVal();
            if (val)
                vec.push_back(val);
        }
        for (uint8_t j = 1; j < vec.size(); ++j)
        {
            if (vec[j - 1] && vec[j - 1] == vec[j])
            {
                ++vec[j - 1];
                vec[j] = 0;
                res += 1 << vec[j - 1];
            }
        }
        for (uint8_t j = 0; j < width; ++j)
        {
            uint8_t val = 0;
            while (!vec.empty() && vec.front() == 0)
                vec.pop_front();
            if (!vec.empty())
            {
                val = vec.front();
                vec.pop_front();
            }
            // printf("setting data[%d][%d] = %d\n", i, j, val);
            data[make_pair(i, j)].SetVal(val);
        }
    }
    // post-process
    switch (direction)
    {
    case 'w':
        RotateRight();
        break;
    case 's':
        RotateLeft();
        break;
    case 'd':
        HorizontalMirror();
        break;
    }
    return res;
}
uint64_t Grid2048::GetBoard()
{
    uint64_t res = 0;
    for (int8_t i = height - 1; i >= 0; --i)
        for (int8_t j = width - 1; j >= 0; --j)
        {
            //qDebug() << Qt::bin << res;
            res <<= 4;
            res |= data[make_pair(i, j)].GetVal() & 0xf;
        }
    return res;
}
bool Grid2048::IsAbleToMerge(uint8_t direction)
{
    if (direction != 'w' && direction != 'a' && direction != 's' && direction != 'd')
        return false;
    auto dataCpy = data;
    //qDebug("sizecmp: %lu %lu", data.size(), dataCpy.size());
    Merge(direction);
    bool res = dataCpy != data;
    data = dataCpy;
    return res;
}
void Grid2048::RotateLeft()
{
    map<PairOfInt8, BaseTile> newData;
    for (int j = 0, _i = 0; _i < height; ++j, ++_i)
    {
        for (int i = 0, _j = width - 1; _j >= 0; ++i, --_j)
            newData[make_pair(i, j)] = data[make_pair(_i, _j)];
    }
    data = newData;
    swap(width, height);
    // printf("\nafter Lrotate:");
    // DebugPrint();
}
void Grid2048::RotateRight()
{
    map<PairOfInt8, BaseTile> newData;
    for (int j = 0, _i = height - 1; _i >= 0; ++j, --_i)
    {
        for (int i = 0, _j = 0; _j < width; ++i, ++_j)
            newData[make_pair(i, j)] = data[make_pair(_i, _j)];
    }
    data = newData;
    swap(width, height);

    // printf("\nafter Rrotate:");
    // DebugPrint();
}
void Grid2048::HorizontalMirror()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0, _j = width - 1; j < _j; ++j, --_j)
            swap(data[make_pair(i, j)], data[make_pair(i, _j)]);

    // printf("\nafter Hmirror:");
    // DebugPrint();
}
Game2048::Game2048(uint8_t _w, uint8_t _h) : grid(_w, _h), score(0)
{
    InitBoard();
    init_tables();
}
bool Game2048::GameOver()
{
    bool merge = grid.IsAbleToMerge('w') || grid.IsAbleToMerge('a') || grid.IsAbleToMerge('s') || grid.IsAbleToMerge('d');
    return !merge;
}
void Game2048::InitBoard()
{
    grid.PutNewTileOnRandomLocation();
    grid.PutNewTileOnRandomLocation();
    //Print();
}
uint8_t Game2048::GetBestMove()
{
    freopen("NUL", "w", stdout);
    uint8_t bestMove = find_best_move(grid.GetBoard());
    freopen("CON", "w", stdout);
    switch (bestMove)
    {
    case 0: // up
        return 'w';
    case 1: // down
        return 's';
    case 2: // left
        return 'a';
    case 3: // right
        return 'd';
    default:
        return 0;
    }
}
void Game2048::Step(uint8_t direction)
{
    if (grid.IsAbleToMerge(direction))
    {
        score += grid.Merge(direction);
        grid.PutNewTileOnRandomLocation();
        //Print();
    }
    //else
    //    qDebug("\nCannot merge in this Direction!");
}
/*
void Grid2048::DebugPrintGrid()
{
    QString LINE;
    for (uint8_t i = 0; i < height; ++i)
    {
        LINE = "+";
        for (uint8_t j = 0; j < width; ++j)
            LINE += "----+";
        qDebug().noquote() << LINE;
        LINE = "|";
        for (uint8_t j = 0; j < width; ++j)
        {
            uint16_t disp = 1 << data[make_pair(i, j)].GetVal();
            char seg[6];
            if (disp == 1)
                sprintf(seg, "    |");
            else if (disp <= 64)
                sprintf(seg, "%3d |", disp);
            else
                sprintf(seg, "%4d|", disp);
            LINE += seg;
        }
        qDebug().noquote() << LINE;
    }
    LINE = "+";
    for (uint8_t j = 0; j < width; ++j)
        LINE += "----+";
    qDebug().noquote() << LINE;
}
void Game2048::Print()
{
    //qDebug() << "-New-Round-";
    if(!GameOver())
    {
        grid.DebugPrintGrid();
        QString MOVE = "N/A";
        uint8_t bestMove = GetBestMove();
        switch (bestMove)
        {
        case 'w': // up
            MOVE = "UP";
            break;
        case 's': // down
            MOVE = "DOWN";
            break;
        case 'a': // left
            MOVE = "LEFT";
            break;
        case 'd': // right
            MOVE = "RIGHT";
            break;
        }
        qDebug() << "Optimal Move:" << MOVE;
    }
    else
    {
        qDebug() << "Game Over!";
    }
    //qDebug() << "input direction: w, a, s, d:";
}*/
