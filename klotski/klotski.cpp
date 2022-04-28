#include "klotski.h"

void GridKlotski::SetTile(uint8_t val, uint8_t i, uint8_t j, uint8_t wid, uint8_t hei)
{
    PairOfInt8 loc = make_pair(i, j);
    data[loc] = TileKlotski(val, wid, hei);
}

GameKlotski::GameKlotski(const QString& gs) : grid(GameSetData[gs][0].toUInt(), GameSetData[gs][1].toUInt())
{
    //qDebug() << GameSetData[gs][0].toUInt() << GameSetData[gs][1].toUInt();
    auto &&GameSet = GameSetData[gs];
    QStringList Tiles = GameSet[2].split('|');
    for(auto &&line:Tiles)
    {
        //qDebug() << line;
        QStringList TileProperties = line.split(',');
        grid.SetTile(TileProperties[0].toUInt(),
                TileProperties[1].toUInt(),
                TileProperties[2].toUInt(),
                TileProperties[3].toUInt(),
                TileProperties[4].toUInt());
    }
}

bool GridKlotski::IsAbleToMove(PairOfInt8 loc, uint8_t direction)
{
    int i = loc.first, j = loc.second;
    for (uint8_t x = i; x < i + GetTileHeight(loc); ++x)
        for (uint8_t y = j; y < j + GetTileWidth(loc); ++y)
            switch (direction)
            {
            case 'w':
                if (x - 1 < 0 || !data[make_pair(x - 1, y)].IsEmpty())
                    return false;
                break;
            case 's':
                if (x + 1 >= height || !data[make_pair(x + 1, y)].IsEmpty())
                    return false;
                break;
            case 'a':
                if (y - 1 < 0 || !data[make_pair(x, y - 1)].IsEmpty())
                    return false;
                break;
            case 'd':
                if (y + 1 >= width || !data[make_pair(x, y + 1)].IsEmpty())
                    return false;
                break;
            default:
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
                if ( ( ( (sy <= y && y < ey) && (ex == x || sx == x + 1) ) || ( (sx <= x && x < ex) && (ey == y || sy == y + 1) ) )
                  && ( (IsAbleToMove(loc, 'w')) || (IsAbleToMove(loc, 'a')) || (IsAbleToMove(loc, 's')) || (IsAbleToMove(loc, 'd')) ) )
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
