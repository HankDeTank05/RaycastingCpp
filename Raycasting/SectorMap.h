#ifndef SECTOR_MAP_H
#define SECTOR_MAP_H

#include <vector>

class SectorMap
{
public:
	SectorMap();

	void AddWall(int x0, int y0, int x1, int y1, bool invisible = false);

	struct Wall
	{
		int x0;
		int y0;
		int x1;
		int y1;
		bool invisible;
	};

private:
	std::vector<SectorMap::Wall> wallList;
};

#endif