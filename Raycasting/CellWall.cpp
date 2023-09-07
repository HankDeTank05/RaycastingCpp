#include "CellWall.h"

CellWall::CellWall(int _texIndex)
	: MapCell(MapCell::Type::Wall),
	texIndex(_texIndex)
{
	// do nothing
}
