#include "2048.h"
Tile::Tile() : val(0) {}
Tile::Tile(int _v) : val(_v) {}
uint8_t Tile::GetVal() { return val; }
void Tile::SetVal(int _v) { val = _v; }
bool Tile::operator==(const Tile &other) const { return val == other.val; }

Grid::Grid(uint8_t _w, uint8_t _h) : width(_w), height(_h) {}
void Grid::DebugPrintGrid()
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
bool Grid::IsFull()
{
    for (uint8_t i = 0; i < height; ++i)
        for (uint8_t j = 0; j < width; ++j)
            if (data[make_pair(i, j)].GetVal() == 0)
                return false;
    return true;
}
void Grid::PutNewTileOnRandomLocation()
{
    if (IsFull())
        return;
    PairOfInt8 location = GetRandomEmptyLocation();
    data[location] = rand() < RAND_MAX * 0.9 ? 1 : 2;
    // printf("\nplacing at [%d][%d]", location.first, location.second);
}
PairOfInt8 Grid::GetRandomEmptyLocation()
{
    if (IsFull())
        return make_pair(-1, -1);
    uint8_t x = rand() % height;
    uint8_t y = rand() % width;
    while (data[make_pair(x, y)].GetVal())
    {
        x = rand() % height;
        y = rand() % width;
    }
    return make_pair(x, y);
}
uint16_t Grid::Merge(uint8_t direction)
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
uint64_t Grid::GetBoard()
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
bool Grid::IsAbleToMerge(uint8_t direction)
{
    map<PairOfInt8, Tile> dataCpy = data;
    Merge(direction);
    bool res = dataCpy != data;
    data = dataCpy;
    // printf("\nsizecmp: %lu %lu, res: %d", data.size(), dataCpy.size(),res);
    return res;
}
void Grid::RotateLeft()
{
    map<PairOfInt8, Tile> newData;
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
void Grid::RotateRight()
{
    map<PairOfInt8, Tile> newData;
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
void Grid::HorizontalMirror()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0, _j = width - 1; j < _j; ++j, --_j)
            swap(data[make_pair(i, j)], data[make_pair(i, _j)]);

    // printf("\nafter Hmirror:");
    // DebugPrint();
}
Game::Game(int _w, int _h) : grid(_w, _h), score(0)
{
    InitBoard();
    init_tables();
}
bool Game::GameOver()
{
    bool merge = grid.IsAbleToMerge('w') || grid.IsAbleToMerge('a') || grid.IsAbleToMerge('s') || grid.IsAbleToMerge('d');
    return !merge;
}
void Game::InitBoard()
{
    grid.PutNewTileOnRandomLocation();
    grid.PutNewTileOnRandomLocation();
    Print();
}
void Game::Print()
{
    //qDebug() << "-New-Round-";
    if(!GameOver())
    {
        qDebug() << "Score:" << score /*<< "Board:" << Qt::bin << grid.GetBoard()*/;
        grid.DebugPrintGrid();

        freopen("NUL", "w", stdout);
        uint8_t optimalDirection = find_best_move(grid.GetBoard());
        freopen("CON", "w", stdout);

        QString MOVE = "N/A";
        switch (optimalDirection)
        {
        case 0: // up
            optimalDirection = 'w';
            MOVE = "UP";
            break;
        case 1: // down
            optimalDirection = 's';
            MOVE = "DOWN";
            break;
        case 2: // left
            optimalDirection = 'a';
            MOVE = "LEFT";
            break;
        case 3: // right
            optimalDirection = 'd';
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
}
void Game::Step(uint8_t direction)
{
    if (grid.IsAbleToMerge(direction))
    {
        score += grid.Merge(direction);
        grid.PutNewTileOnRandomLocation();
        Print();
    }
    //else
    //    qDebug("\nCannot merge in this Direction!");
}
