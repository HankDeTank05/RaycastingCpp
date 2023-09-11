#ifndef SECTOR_MAP_H
#define SECTOR_MAP_H

#include <vector>

// forward declarations
class Sector;

class SectorMap
{
public:
	SectorMap();

	void AddSector(Sector* pSector);

private:
	std::vector<Sector*> wallList;
};

#endif