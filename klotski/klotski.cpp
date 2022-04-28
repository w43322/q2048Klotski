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
