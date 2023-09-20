#define OLC_PGE_APPLICATION

#include "Raycasting.h"
#include "BinarySpacePartitioning.h"

int main()
{
	/*
	Raycasting game;
	//*/

	//*
	BinarySpacePartitioning game;
	//*/

	if (game.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE_WIDTH, SCREEN_SCALE_HEIGHT))
	{
		game.Start();
	}


	return 0;
}