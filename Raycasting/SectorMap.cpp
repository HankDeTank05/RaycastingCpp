#include "SectorMap.h"

SectorMap::SectorMap()
	: wallList()
{
	// do nothing
}

void SectorMap::AddWall(int x0, int y0, int x1, int y1, bool invisible)
{
	Wall newWall = { x0, y0, x1, y1, invisible };
	wallList.push_back(newWall);
}
