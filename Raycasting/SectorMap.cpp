#include "SectorMap.h"

#include "BSPNode.h"
#include <cassert>

SectorMap::SectorMap()
	: linedefs(),
	treeRoot(nullptr)
{
	// do nothing
}

void SectorMap::AddLineDef(LineDef newLineDef)
{
	linedefs.push_back(newLineDef);
}

std::vector<LineDef> SectorMap::GetLineDefs() const
{
	return linedefs;
}
