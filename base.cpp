#include "base.h"
BaseTile::BaseTile() : val(0) {}
BaseTile::BaseTile(int _v) : val(_v) {}
uint8_t BaseTile::GetVal() { return val; }
void BaseTile::SetVal(int _v) { val = _v; }
bool BaseTile::operator==(const BaseTile &other) const { return val == other.val; }

BaseGrid::BaseGrid(uint8_t _w, uint8_t _h) : width(_w), height(_h) {}

void BaseGrid::RotateLeft()
{
    map<PairOfInt8, BaseTile> newData;
    for (int j = 0, _i = 0; _i < height; ++j, ++_i)
    {
        for (int i = 0, _j = width - 1; _j >= 0; ++i, --_j)
            newData[make_pair(i, j)] = data[make_pair(_i, _j)];
    }
    data = newData;
    swap(width, height);
    // printf("\nafter Lrotate:");
    // DebugPrint();
}
void BaseGrid::RotateRight()
{
    map<PairOfInt8, BaseTile> newData;
    for (int j = 0, _i = height - 1; _i >= 0; ++j, --_i)
    {
        for (int i = 0, _j = 0; _j < width; ++i, ++_j)
            newData[make_pair(i, j)] = data[make_pair(_i, _j)];
    }
    data = newData;
    swap(width, height);

    // printf("\nafter Rrotate:");
    // DebugPrint();
}
void BaseGrid::HorizontalMirror()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0, _j = width - 1; j < _j; ++j, --_j)
            swap(data[make_pair(i, j)], data[make_pair(i, _j)]);

    // printf("\nafter Hmirror:");
    // DebugPrint();
}
