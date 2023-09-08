#include "CellOpen.h"

CellOpen::~CellOpen()
{
	// do nothing
}

CellOpen::CellOpen(int _floorTexIndex, int _ceilingTexIndex)
	: MapCell(MapCell::Type::Open),
	floorTexIndex(_floorTexIndex),
	ceilingTexIndex(_ceilingTexIndex)
{
	// do nothing
}

int CellOpen::GetFloorTexIndex() const
{
	return floorTexIndex;
}

int CellOpen::GetCeilingTexIndex() const
{
	return ceilingTexIndex;
}
