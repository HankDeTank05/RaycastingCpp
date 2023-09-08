#ifndef CELL_WALL_H
#define CELL_WALL_H

#include "MapCell.h"

class CellWall : public MapCell
{
public:
	CellWall() = delete;
	CellWall(const CellWall& cw) = delete;
	CellWall& operator=(const CellWall& cw) = delete;
	virtual ~CellWall();

	CellWall(int texIndex);

	int GetTexIndex() const;

private:
	int texIndex;
};

#endif