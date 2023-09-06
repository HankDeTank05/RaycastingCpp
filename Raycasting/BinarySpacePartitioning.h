//#ifndef BINARY_SPACE_PARTITIONING_H
//#define BINARY_SPACE_PARTITIONING_H
//
//#include "olcPixelGameEngine.h"
//#include "SectorMap.h"
//
//class BinarySpacePartitioning : public olc::PixelGameEngine
//{
//public:
//	BinarySpacePartitioning()
//	{
//		sAppName = "Binary Space Partitioning";
//	}
//
//private:
//	SectorMap worldMap;
//
//	double posX, posY; // x and y start position
//	double dirX, dirY; // initial direction vector
//	double planeX, planeY; // the 2d raycaster version of camera plane
//
//public:
//	bool OnUserCreate() override
//	{
//		// called once at the start, so create things here
//
//		worldMap = SectorMap();
//
//		// sector 1
//		worldMap.AddWall(4, 8, 7, 8);
//		worldMap.AddWall(7, 8, 9, 6); // will be invisible later, leads to sector 3
//		worldMap.AddWall(9, 6, 9, 3);
//		worldMap.AddWall(9, 3, 7, 1);
//		worldMap.AddWall(7, 1, 4, 1);
//		worldMap.AddWall(4, 1, 2, 3);
//		worldMap.AddWall(2, 3, 2, 6);
//		worldMap.AddWall(2, 6, 4, 8); // will be invisible later, leads to sector 2
//
//		// x and y start position
//		posX = 6, posY = 6;
//
//		// initial direction vector
//		dirX = 0; dirY = 1;
//
//		// the 2d version of a camera plane
//		planeX = 0; planeY = 0.66;
//	}
//
//	bool OnUserUpdate(float fElapsedTime) override
//	{
//
//	}
//};
//
//#endif