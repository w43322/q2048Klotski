#include "base.h"
BaseTile::BaseTile() : val(0) {}
BaseTile::BaseTile(uint8_t _v) : val(_v) {}
uint8_t BaseTile::GetVal() const { return val; }
void BaseTile::SetVal(uint8_t _v) { val = _v; }
bool BaseTile::operator==(const BaseTile &other) const { return val == other.val; }

BaseGrid::BaseGrid(uint8_t _w, uint8_t _h) : width(_w), height(_h) {}
