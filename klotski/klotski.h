#ifndef KLOTSKI_H
#define KLOTSKI_H

#include "base/base.h"

#include <QStringList>
#include <QDebug>

class TileKlotski : public BaseTile
{
public:
    TileKlotski() : BaseTile(0), wid(0), hei(0){};
    TileKlotski(uint8_t _val, uint8_t _wid, uint8_t _hei) : BaseTile(_val), wid(_wid), hei(_hei){};
    uint8_t GetTileWidth(){return wid;}
    uint8_t GetTileHeight(){return hei;}
    bool IsEmpty(){return isEmpty;}
    bool IsAdj(){return isAdj;}
    void SetEmpty(const bool &s){isEmpty = s;}
    void SetAdj(const bool &s){isAdj = s;}
private:
    uint8_t wid;
    uint8_t hei;
    bool isEmpty;
    bool isAdj;
};

class GridKlotski : public BaseGrid
{
public:
    //void DebugPrintGrid();
    GridKlotski(uint8_t _w, uint8_t _h): BaseGrid(_w, _h){}
    void SetTile(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
    uint8_t GetTileVal(PairOfInt8 loc){return data[loc].GetVal();}
    uint8_t GetTileWidth(PairOfInt8 loc){return data[loc].GetTileWidth();}
    uint8_t GetTileHeight(PairOfInt8 loc){return data[loc].GetTileHeight();}
    bool TileIsAdj(PairOfInt8 loc){return data[loc].IsAdj();}
    void UpdateEmpty();
    void UpdateAdj();
    void ClearGrid(){data.clear();}
    bool IsAbleToMove(PairOfInt8 loc, uint8_t direction);
    PairOfInt8 Move(PairOfInt8 loc, uint8_t direction);
private:
    map<PairOfInt8, TileKlotski> data;
};

class GameKlotski
{
public:
    GameKlotski(const QString&);
    //void Print();
    uint32_t GetStep(){return step;}
    uint8_t GetWidth(){return grid.GetWidth();}
    uint8_t GetHeight(){return grid.GetHeight();}
    uint8_t GetTileVal(PairOfInt8 loc){return grid.GetTileVal(loc);}
    uint8_t GetTileWidth(PairOfInt8 loc){return grid.GetTileWidth(loc);}
    uint8_t GetTileHeight(PairOfInt8 loc){return grid.GetTileHeight(loc);}
    bool TileIsAdj(PairOfInt8 loc){return grid.TileIsAdj(loc);}
    bool DataIsInitialized(){return !GameSetData.empty();}
    void Update(){grid.UpdateEmpty();grid.UpdateAdj();}
    PairOfInt8 Move(PairOfInt8 loc, uint8_t direction); // returns the new location
    PairOfInt8 NextAdjUp(PairOfInt8 loc);
    PairOfInt8 NextAdjDown(PairOfInt8 loc);
    PairOfInt8 NextAdjLeft(PairOfInt8 loc);
    PairOfInt8 NextAdjRight(PairOfInt8 loc);
    map<QString, QStringList> GameSetData =
    {
        {"横刀立马",{"4", "5", "1,0,0,1,2|0,0,1,2,2|1,0,3,1,2|1,2,0,1,2|2,2,1,2,1|1,2,3,1,2|3,3,1,1,1|3,3,2,1,1|3,4,0,1,1|3,4,3,1,1", "0,3,1"}},
        {"指挥若定",{"4", "5", "1,0,0,1,2|0,0,1,2,2|1,0,3,1,2|3,2,0,1,1|2,2,1,2,1|3,2,3,1,1|1,3,0,1,2|3,3,1,1,1|3,3,2,1,1|1,3,3,1,2", "0,3,1"}},
        {"数字华容道",{"4", "4", "NUMBERRAND", "NUMBERWIN"}}
    };
private:
    GridKlotski grid;
    uint32_t step = 0;
};
#endif // KLOTSKI_H
