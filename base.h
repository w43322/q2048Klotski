#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <map>
#include <deque>

using std::deque;
using std::make_pair;
using std::map;
using std::pair;
using std::swap;
using std::max;

typedef std::pair<uint8_t, uint8_t> PairOfInt8;
typedef std::map<PairOfInt8, class BaseTile> MapOfTile;

class BaseTile
{
public:
    BaseTile();
    BaseTile(int _v);
    uint8_t GetVal();
    void SetVal(int _v);
    bool operator==(const BaseTile &other) const;
private:
    uint8_t val;
    friend class Tile;
};

class BaseGrid
{
public:
    BaseGrid(uint8_t _w, uint8_t _h);
    uint8_t GetWidth(){return width;}
    uint8_t GetHeight(){return height;}
    uint8_t GetTileVal(PairOfInt8 loc){return data[loc].GetVal();}
private:
    void RotateLeft();
    void RotateRight();
    void HorizontalMirror();
    uint8_t width;
    uint8_t height;
    MapOfTile data;
    friend class Grid;
};

#endif // BASE_H
