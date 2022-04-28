#ifndef KLOTSKI_H
#define KLOTSKI_H

#include "base/base.h"

#include <QStringList>

/*map<QString, QStringList> GameSetData;
void InitGameSetData()
{
    if (!GameSetData.empty())
        return;
    GameSetData["横刀立马"] = (QStringList){"1","2","3"};
}*/

class TileKlotski : public BaseTile
{
public:
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
};

class GameKlotski
{
public:
    //GameKlotski(const QString&);
    //void Print();
    uint32_t GetStep(){return step;}
    uint8_t GetWidth(){return grid.GetWidth();}
    uint8_t GetHeight(){return grid.GetHeight();}
    uint8_t GetTileVal(PairOfInt8 loc){return grid.GetTileVal(loc);}
    uint8_t GetBestMove();
private:
    GridKlotski grid;
    uint32_t step = 0;
};
#endif // KLOTSKI_H
