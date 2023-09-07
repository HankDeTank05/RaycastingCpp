#include "MapCell.h"

MapCell::~MapCell()
{
	// do nothing
}

MapCell::MapCell(Type _cellType)
	: cellType(_cellType)
{
	// do nothing
}

MapCell::Type MapCell::GetCellType() const
{
	return cellType;
}
