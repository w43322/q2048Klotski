#include "klotski.h"

const QStringList GameKlotski::GameSetStringList =
{
    "横刀立马",
    "指挥若定",
    "数字华容道",
    "将拥曹营",
    "齐头并进",
    "兵分三路",
    "雨声淅沥",
    "左右布兵",
    "桃花园中",
    "一路进军",
    "一路顺风",
    "围而不歼",
    "捷足先登"
};

const map<QString, QStringList> GameKlotski::GameSetData =
{
   //          wid  hei  number, row, col, wid, hei | ...                                             win condition - num, (at) row, col
   {"横刀立马",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,0,3,1,2|1,2,0,1,2|2,2,1,2,1|1,2,3,1,2|3,3,1,1,1|3,3,2,1,1|3,4,0,1,1|3,4,3,1,1", "4,3,1"}},
   {"指挥若定",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,0,3,1,2|3,2,0,1,1|2,2,1,2,1|3,2,3,1,1|1,3,0,1,2|3,3,1,1,1|3,3,2,1,1|1,3,3,1,2", "4,3,1"}},
   {"数字华容道",{"4", "4", "NUMBERRAND", "1,0,0|2,0,1|3,0,2|4,0,3|5,1,0|6,1,1|7,1,2|8,1,3|9,2,0|10,2,1|11,2,2|12,2,3|13,3,0|14,3,1|15,3,2"}},
   {"将拥曹营",{"4", "5", "1,1,0,1,2|4,0,1,2,2|1,1,3,1,2|2,4,0,2,1|3,3,0,1,1|3,4,2,1,1|3,3,3,1,1|3,4,3,1,1|1,2,1,1,2|1,2,2,1,2", "4,3,1"}},
   {"齐头并进",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,0,3,1,2|1,3,0,1,2|1,3,3,1,2|2,3,1,2,1|3,2,0,1,1|3,2,1,1,1|3,2,2,1,1|3,2,3,1,1", "4,3,1"}},
   {"兵分三路",{"4", "5", "1,1,0,1,2|4,0,1,2,2|1,3,0,1,2|1,0,3,1,2|1,3,3,1,2|2,2,1,2,1|3,0,0,1,1|3,0,3,1,1|3,3,1,1,1|3,3,2,1,1", "4,3,1"}},
   {"雨声淅沥",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,2,0,1,2|1,3,1,1,2|1,2,3,1,2|2,2,1,2,1|3,0,3,1,1|3,4,0,1,1|3,4,3,1,1|3,1,3,1,1", "4,3,1"}},
   {"左右布兵",{"4", "5", "1,2,0,1,2|4,0,1,2,2|1,2,1,1,2|1,2,2,1,2|1,2,3,1,2|2,4,1,2,1|3,0,0,1,1|3,1,0,1,1|3,0,3,1,1|3,1,3,1,1", "4,3,1"}},
   {"桃花园中",{"4", "5", "1,1,0,1,2|4,0,1,2,2|1,2,1,1,2|1,2,2,1,2|1,1,3,1,2|2,4,1,2,1|3,0,0,1,1|3,0,3,1,1|3,3,0,1,1|3,3,3,1,1", "4,3,1"}},
   {"一路进军",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,2,0,1,2|1,2,1,1,2|1,2,2,1,2|2,4,1,2,1|3,0,3,1,1|3,1,3,1,1|3,2,3,1,1|3,3,3,1,1", "4,3,1"}},
   {"一路顺风",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,2,0,1,2|1,2,3,1,2|1,3,2,1,2|2,2,1,2,1|3,3,1,1,1|3,4,1,1,1|3,0,3,1,1|3,1,3,1,1", "4,3,1"}},
   {"围而不歼",{"4", "5", "1,0,0,1,2|4,0,1,2,2|1,2,0,1,2|1,3,2,1,2|1,3,1,1,2|2,2,1,2,1|3,0,3,1,1|3,1,3,1,1|3,2,3,1,1|3,3,3,1,1", "4,3,1"}},
   {"捷足先登",{"4", "5", "1,3,0,1,2|4,0,1,2,2|1,3,1,1,2|1,3,2,1,2|1,3,3,1,2|2,2,1,2,1|3,0,0,1,1|3,1,0,1,1|3,0,3,1,1|3,1,3,1,1", "4,3,1"}}
};

void GridKlotski::SetTile(uint8_t val, uint8_t i, uint8_t j, uint8_t wid, uint8_t hei)
{
    PairOfInt8 loc = make_pair(i, j);
    data[loc] = TileKlotski(val, wid, hei);
}

GameKlotski::GameKlotski(const QString& gs) : grid(GameSetData.at(gs)[0].toUInt(), GameSetData.at(gs)[1].toUInt())
{
    // Set Initial Grid
    grid.ClearGrid();
    auto&& GameSet = GameSetData.at(gs);
    if (GameSet[2] == "NUMBERRAND")
    {
        for (uint8_t num = 1; num < GetWidth() * GetHeight(); ++num)
        {
            uint8_t i = rand() % GetHeight();
            uint8_t j = rand() % GetWidth();
            while (grid.GetTileVal(make_pair(i, j)) != 0)
            {
                i = rand() % GetHeight();
                j = rand() % GetWidth();
            }
            grid.SetTile(num, i, j, 1, 1);
        }
    }
    else
    {
        QStringList Tiles = GameSet[2].split('|');
        for (auto&& line : Tiles)
        {
            QStringList TileProperties = line.split(',');
            grid.SetTile(TileProperties[0].toUInt(),
                    TileProperties[1].toUInt(),
                    TileProperties[2].toUInt(),
                    TileProperties[3].toUInt(),
                    TileProperties[4].toUInt());
        }
    }

    // Set Win Condition
    QStringList winConds = GameSet[3].split('|');
    for (auto&& winCond : winConds)
    {
        QStringList winCondPart = winCond.split(',');
        winCondition.push_back(std::make_tuple(
                                   winCondPart[0].toUInt(),
                                   winCondPart[1].toUInt(),
                                   winCondPart[2].toUInt()));
    }
}

bool GridKlotski::IsAbleToMove(PairOfInt8 loc, uint8_t direction)
{
    uint8_t i = loc.first, j = loc.second;
    uint8_t sx = i, sy = j, ex = i + GetTileHeight(loc), ey = j + GetTileWidth(loc);
    for (uint8_t x = sx; x < ex; ++x)
        for (uint8_t y = sy; y < ey; ++y)
        {
            uint8_t nx = x, ny = y;
            switch (direction)
            {
            case 'w':
                nx = x - 1;
                break;
            case 's':
                nx = x + 1;
                break;
            case 'a':
                ny = y - 1;
                break;
            case 'd':
                ny = y + 1;
                break;
            default:
                return false;
            }
            if (nx == 255 || ny == 255 || nx >= height || ny >= width)
                return false;
            if (!data[make_pair(nx, ny)].IsEmpty())
                if (nx < sx || ny < sy || nx >= ex || ny >= ey)
                    return false;
        }
    return true;
}

void GridKlotski::UpdateEmpty()
{
    for (uint8_t i = 0; i < GetHeight(); ++i)
        for (uint8_t j = 0; j < GetWidth(); ++j)
            data[make_pair(i, j)].SetEmpty(true);
    for (uint8_t i = 0; i < GetHeight(); ++i)
        for (uint8_t j = 0; j < GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            for (uint8_t x = i; x < i + GetTileHeight(loc); ++x)
                for (uint8_t y = j; y < j + GetTileWidth(loc); ++y)
                    data[make_pair(x, y)].SetEmpty(false);
        }
}

void GridKlotski::UpdateAdj()
{
    deque<PairOfInt8> EmptyLocations;
    for (uint8_t i = 0; i < GetHeight(); ++i)
        for (uint8_t j = 0; j < GetWidth(); ++j)
        {
            PairOfInt8 loc = make_pair(i, j);
            data[loc].SetAdj(false);
            if (data[loc].IsEmpty())
                EmptyLocations.push_back(loc);
        }
    for (auto&& EmptyLocation:EmptyLocations)
    {
        for (uint8_t i = 0; i < GetHeight(); ++i)
            for (uint8_t j = 0; j < GetWidth(); ++j)
            {
                PairOfInt8 loc = make_pair(i, j);
                uint8_t sx = i, sy = j, ex = i + GetTileHeight(loc), ey = j + GetTileWidth(loc);
                uint8_t x = EmptyLocation.first, y = EmptyLocation.second;
                if  ( ( (sy <= y && y < ey) && (ex == x || sx == x + 1) ) || ( (sx <= x && x < ex) && (ey == y || sy == y + 1) ) )
                    if ( (IsAbleToMove(loc, 'w')) || (IsAbleToMove(loc, 'a')) || (IsAbleToMove(loc, 's')) || (IsAbleToMove(loc, 'd')) )
                        data[loc].SetAdj(true);
            }
    }
}

PairOfInt8 GameKlotski::Move(PairOfInt8 loc, uint8_t direction)
{
    PairOfInt8 res = loc;
    if (grid.IsAbleToMove(loc, direction))
    {
        ++step;
        res = grid.Move(loc, direction);
        Update();
    }
    return res;
}

PairOfInt8 GridKlotski::Move(PairOfInt8 loc, uint8_t direction)
{
    int i = loc.first, j = loc.second;
    uint8_t nx = i, ny = j;
    switch (direction)
    {
    case 'w':
        nx = i - 1;
        break;
    case 's':
        nx = i + 1;
        break;
    case 'a':
        ny = j - 1;
        break;
    case 'd':
        ny = j + 1;
        break;
    default:
        return loc;
    }
    PairOfInt8 newloc = make_pair(nx, ny);
    data[newloc] = data[loc];
    for (uint8_t x = i; x < i + GetTileHeight(loc); ++x)
        for (uint8_t y = j; y < j + GetTileWidth(loc); ++y)
            data[make_pair(x, y)] = TileKlotski();
    return newloc;
}

PairOfInt8 GameKlotski::NextAdjLeft(PairOfInt8 loc)
{
    uint8_t wid = GetWidth(), hei = GetHeight();
    int8_t i = loc.first - 1, j = loc.second;
    for (; ; j = (j + wid - 1) % wid)
    {
        for (; i >= 0; --i)
        {
            PairOfInt8 res = make_pair(i, j);
            if (grid.TileIsAdj(res))
                return res;
        }
        i = hei - 1;
    }
}

PairOfInt8 GameKlotski::NextAdjRight(PairOfInt8 loc)
{
    uint8_t wid = GetWidth(), hei = GetHeight();
    int8_t i = loc.first + 1, j = loc.second;
    for (; ; j = (j + 1) % wid)
    {
        for (; i < hei; ++i)
        {
            PairOfInt8 res = make_pair(i, j);
            if (grid.TileIsAdj(res))
                return res;
        }
        i = 0;
    }
}

PairOfInt8 GameKlotski::NextAdjUp(PairOfInt8 loc)
{
    uint8_t wid = GetWidth(), hei = GetHeight();
    int8_t i = loc.first, j = loc.second - 1;
    for (; ; i = (i + hei - 1) % hei)
    {
        for (; j >= 0; --j)
        {
            PairOfInt8 res = make_pair(i, j);
            if (grid.TileIsAdj(res))
                return res;
        }
        j = wid - 1;
    }
}

PairOfInt8 GameKlotski::NextAdjDown(PairOfInt8 loc)
{
    uint8_t wid = GetWidth(), hei = GetHeight();
    int8_t i = loc.first, j = loc.second + 1;
    for (; ; i = (i + 1) % hei)
    {
        for (; j < wid; ++j)
        {
            PairOfInt8 res = make_pair(i, j);
            if (grid.TileIsAdj(res))
                return res;
        }
        j = 0;
    }
}

bool GameKlotski::GameOver()
{
    for (auto&& winCond : winCondition)
    {
        auto loc = make_pair(std::get<1>(winCond), std::get<2>(winCond));
        if (grid.GetTileVal(loc) != std::get<0>(winCond))
            return false;
        //qDebug() << loc;
    }
    return true;
}
