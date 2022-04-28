#ifndef _2048_H
#define _2048_H

#include "base/base.h"
#include "2048-ai/2048ai.h"

class Grid2048 : public BaseGrid
{
public:
    //void DebugPrintGrid();
    Grid2048(uint8_t _w, uint8_t _h): BaseGrid(_w, _h){}
    bool IsFull();
    void PutNewTileOnRandomLocation();
    PairOfInt8 GetRandomEmptyLocation();
    uint16_t Merge(uint8_t direction);
    bool IsAbleToMerge(uint8_t direction);
    uint64_t GetBoard();
    void RotateLeft();
    void RotateRight();
    void HorizontalMirror();
    uint8_t GetTileVal(PairOfInt8 loc){return data[loc].GetVal();}
private:
    map<PairOfInt8, BaseTile> data;
};

class Game2048
{
public:
    Game2048(uint8_t _w, uint8_t _h);
    bool GameOver();
    void InitBoard();
    //void Print();
    void Step(uint8_t direction);
    uint32_t GetScore(){return score;}
    uint8_t GetWidth(){return grid.GetWidth();}
    uint8_t GetHeight(){return grid.GetHeight();}
    uint8_t GetTileVal(PairOfInt8 loc){return grid.GetTileVal(loc);}
    uint8_t GetBestMove();
private:
    Grid2048 grid;
    uint32_t score = 0;
};
#endif // _2048_H
