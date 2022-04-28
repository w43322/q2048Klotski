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
private:
    uint8_t wid;
    uint8_t hei;
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
    bool DataIsInitialized(){return !GameSetData.empty();}
    map<QString, QStringList> GameSetData =
    {
        {"横刀立马",{"4", "5", "1,0,0,1,2|0,0,1,2,2|1,0,3,1,2|1,2,0,1,2|2,2,1,2,1|1,2,3,1,2|3,3,1,1,1|3,3,2,1,1|3,4,0,1,1|3,4,3,1,1", "0,3,1"}}
    };
private:
    GridKlotski grid;
    uint32_t step = 0;
};
#endif // KLOTSKI_H
