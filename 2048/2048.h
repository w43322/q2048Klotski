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
    uint8_t GetTileVal(PairOfInt8 loc) const{return data.find(loc) == data.end() ? 0 : data.at(loc).GetVal();}
    void SetTileVal(PairOfInt8 loc, uint8_t val){data[loc].SetVal(val);}
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
    //const Grid2048& GetGrid(){return grid;}
    uint8_t GetTileVal(PairOfInt8 loc){return grid.GetTileVal(loc);}
    uint8_t GetBestMove();
private:
    Grid2048 grid;
    uint32_t score = 0;
};
#endif // _2048_H
