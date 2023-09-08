#include "CellWall.h"

CellWall::~CellWall()
{
	// do nothing
}

CellWall::CellWall(int _texIndex)
	: MapCell(MapCell::Type::Wall),
	texIndex(_texIndex)
{
	// do nothing
}

int CellWall::GetTexIndex() const
{
	return texIndex;
}
