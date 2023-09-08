#include "RaycastMap.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Variables.h"

#include "MapCell.h"
#include "CellWall.h"
#include "CellOpen.h"

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

void RaycastMap::LoadMapFromText(std::string levelName)
{
	std::ifstream wallData("../" + levelName + "_walls.csv");

	std::string textLine;

	// for each line in the file
	int currentRow = 0;
	while (std::getline(wallData, textLine))
	{
		if (textLine.at(0) != ',')
		{
			// create a row vector
			grid.push_back(std::vector<MapCell*>());

			// for each character in the line
			for (std::string::iterator it = textLine.begin(); it != textLine.end(); it++)
			{
				if (*it != ',' && *it != '\n')
				{
					std::stringstream str;
					str << *it;
					int texID;
					str >> texID;
					if (texID > 0 && texID < 10)
					{
						grid[currentRow].push_back(new CellWall(texID));
					}
					else if (texID == 0)
					{
						grid[currentRow].push_back(new CellOpen(FLOOR_TEXTURE, CEILING_TEXTURE));
					}
				}
			}
			currentRow++;
		}
	}
}

MapCell* RaycastMap::GetCell(int cellX, int cellY)
{
	//if (cellX < 0) cellX = 0;
	//else if (cellX >= grid.size()) cellX = grid.size() - 1;

	//if (cellY < 0) cellY = 0;
	//else if (cellY >= grid[cellX].size()) cellY = grid[cellX].size() - 1;

	return grid[cellX][cellY];
}
