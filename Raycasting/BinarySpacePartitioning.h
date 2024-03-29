#ifndef BINARY_SPACE_PARTITIONING_H
#define BINARY_SPACE_PARTITIONING_H

#include <map>

#include "olcPixelGameEngine.h"

#include "Variables.h"

#include "SectorMap.h"
#include "Vector4.h"

class BinarySpacePartitioning : public olc::PixelGameEngine
{
public:
	BinarySpacePartitioning();
	BinarySpacePartitioning(const BinarySpacePartitioning& bsp) = delete;
	BinarySpacePartitioning& operator=(const BinarySpacePartitioning& bsp) = delete;
	virtual ~BinarySpacePartitioning();

public:
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;

private: // internal helper functions
	int BruteForceSearchForX(float dotProduct, bool leftSideOfScreen);

private:
	std::array<float, SCREEN_WIDTH / 2 + 1> angleToX;
	SectorMap worldMap;
	Vector4 pos;
	Vector4 dir;
	Vector4 plane;
};

#endif