#ifndef RAYCAST_MAP_H
#define RAYCAST_MAP_H

#include <vector>
#include <string>

// forward declarations
class MapCell;

class RaycastMap
{
public:
	RaycastMap();
	RaycastMap(const RaycastMap& r) = delete;
	RaycastMap& operator=(const RaycastMap& r) = delete;
	virtual ~RaycastMap();

	// ideas
	//	- read from text file
	//	- read from excel/csv file

	void LoadMapFromCSV(std::string levelName);

	MapCell* GetCell(int cellX, int cellY);

private: // internal helper functions
	int ConvertCharToInt(char chr);

private:
	std::vector<std::vector<MapCell*>> grid;
};

#endif