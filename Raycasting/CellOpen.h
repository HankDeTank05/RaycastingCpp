#ifndef CELL_OPEN_H
#define CELL_OPEN_H

#include "MapCell.h"

class CellOpen : public MapCell
{
public:
	CellOpen() = delete;
	CellOpen(const CellOpen& co) = delete;
	CellOpen& operator=(const CellOpen& co) = delete;
	virtual ~CellOpen();

	CellOpen(int floorTexIndex, int ceilingTexIndex);

	int GetFloorTexIndex() const;
	int GetCeilingTexIndex() const;

private:
	int floorTexIndex;
	int ceilingTexIndex;
};

#endif