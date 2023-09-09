#include "RaycastMap.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Variables.h"

#include "MapCell.h"
#include "CellWall.h"
#include "CellOpen.h"
#include <cassert>

RaycastMap::RaycastMap()
	: grid()
{
	// do nothing
}

RaycastMap::~RaycastMap()
{
	for (std::vector<std::vector<MapCell*>>::iterator it1 = grid.begin(); it1 != grid.end(); it1++)
	{
		for (std::vector<MapCell*>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			delete *it2;
		}
	}
}

void RaycastMap::LoadMapFromCSV(std::string levelName)
{
	std::string fileLocation = "Resources/maps/";
	std::string suffix = "_walls";
	std::string fileType = ".csv";

	std::vector<int> tempFloorData;
	std::vector<int> tempCeilingData;
	int emptyCellCount = 0;

	// 1. process wall data file

	std::ifstream wallData(fileLocation + levelName + suffix + fileType);
	std::string textLine;

	// for each line in the file...
	int currentRow = 0;
	while (std::getline(wallData, textLine))
	{
		if (textLine.at(0) != ',')
		{
			// create a row vector
			grid.push_back(std::vector<MapCell*>());

			// for each character in the line...
			for (std::string::iterator it = textLine.begin(); it != textLine.end(); it++)
			{
				if (*it != ',' && *it != '\n')
				{
					int texID = ConvertCharToInt(*it);

					assert(texID >= 0);
					assert(texID < 10);

					if (texID > 0 && texID < 10)
					{
						grid[currentRow].push_back(new CellWall(texID));
					}
					else if (texID == 0)
					{
						// TODO: add file processing for floor and ceiling texture data

						int floorTexID;
						int ceilingTexID;

						// TODO: replace constant floor/ceiling texIDs with file-loaded ones
						//grid[currentRow].push_back(new CellOpen(FLOOR_TEXTURE, CEILING_TEXTURE));
						grid[currentRow].push_back(nullptr);
						emptyCellCount++;
					}
				}
			}
			currentRow++;
		}
	}
	wallData.close();

	// 2. process floor data file

	suffix = "_floors";
	std::ifstream floorData(fileLocation + levelName + suffix + fileType);

	while (std::getline(floorData, textLine))
	{
		for (std::string::iterator it = textLine.begin(); it != textLine.end(); it++)
		{
			if (*it != ',' && *it != '\n')
			{
				int floorTexID = ConvertCharToInt(*it);
				tempFloorData.push_back(floorTexID);
			}
		}
	}
	floorData.close();
	assert(tempFloorData.size() == emptyCellCount);

	// 3. process ceiling data file

	suffix = "_ceilings";
	std::ifstream ceilingData(fileLocation + levelName + suffix + fileType);

	while (std::getline(ceilingData, textLine))
	{
		for (std::string::iterator it = textLine.begin(); it != textLine.end(); it++)
		{
			if (*it != ',' && *it != '\n')
			{
				int ceilingTexID = ConvertCharToInt(*it);
				tempCeilingData.push_back(ceilingTexID);
			}
		}
	}
	ceilingData.close();
	assert(tempCeilingData.size() == emptyCellCount);

	// code goes here to combine temp floor/ceiling data
	// go thru grid: for every nullptr, combine the two texIDs at index[0] into a mapcell
	// pop the IDs from the front of each vector, rinse and repeat

	int texIDindex = 0;
	for (int x = 0; x < grid.size(); x++)
	{
		for (int y = 0; y < grid[x].size(); y++)
		{
			if (grid[x][y] == nullptr)
			{
				grid[x][y] = new CellOpen(tempFloorData[texIDindex], tempCeilingData[texIDindex]);
				texIDindex++;
			}
		}
	}
}

MapCell* RaycastMap::GetCell(int cellX, int cellY)
{
	bool btmX = cellX >= 0;
	bool topX = cellX < grid.size();

	if (btmX && topX)
	{
		bool btmY = cellY >= 0;
		bool topY = cellY < grid[cellX].size();

		if (btmY && topY)
		{
			return grid[cellX][cellY];
		}
	}
	
	return nullptr;
}

int RaycastMap::ConvertCharToInt(char chr)
{
	// convert from numeric string to int

	int num;

	std::stringstream str;
	str << chr;
	str >> num;

	return num;
}
