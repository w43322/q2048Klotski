#ifndef _2048_H
#define _2048_H
#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <deque>
#include <QDebug>
#include <QProcess>

#include "2048-ai/2048ai.h"

using std::cin;
using std::deque;
using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::swap;
using std::max;

typedef std::pair<uint8_t, uint8_t> PairOfInt8;
typedef std::map<PairOfInt8, class Tile> MapOfTile;

class Tile
{
public:
    Tile();
    Tile(int _v);
    uint8_t GetVal();
    void SetVal(int _v);
    bool operator==(const Tile &other) const;
private:
    uint8_t val;
};

class Grid
{
public:
    Grid(uint8_t _w, uint8_t _h);
    void DebugPrintGrid();
    bool IsFull();
    void PutNewTileOnRandomLocation();
    PairOfInt8 GetRandomEmptyLocation();
    uint16_t Merge(uint8_t direction);
    bool IsAbleToMerge(uint8_t direction);
    uint64_t GetBoard();
private:
    void RotateLeft();
    void RotateRight();
    void HorizontalMirror();
    uint8_t width;
    uint8_t height;
    MapOfTile data;
};
class Game
{
public:
    Game(int _w, int _h);
    bool GameOver();
    void InitBoard();
    void Print();
    void Step(uint8_t direction);
private:
    Grid grid;
    uint32_t score;
};
#endif // _2048_H
