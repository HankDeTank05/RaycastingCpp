#ifndef SECTOR_H
#define SECTOR_H

#include <vector>

#include "Vector3.h"

class Sector
{
public:
	Sector();
	Sector(const Sector& s) = delete;
	Sector& operator=(const Sector& s) = delete;
	virtual ~Sector();

	void AddWall(int x0, int y0, int x1, int y1, float nx, float ny);

	struct Wall
	{
		int x0;
		int y0;
		int x1;
		int y1;
		Vector3 normal;
	};

private:
	std::vector<Wall> walls;
};

#endif