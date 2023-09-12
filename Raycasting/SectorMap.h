#ifndef SECTOR_MAP_H
#define SECTOR_MAP_H

#include <vector>

#include "LineDef.h"

// forward declarations
class BSPNode;

class SectorMap
{
public:
	SectorMap();

	void AddLineDef(LineDef newLineDef);
	std::vector<LineDef> GetLineDefs() const;

private:
	std::vector<LineDef> linedefs;
	BSPNode* treeRoot;
};

#endif