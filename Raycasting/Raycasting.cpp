#include "Raycasting.h"

#include "Player.h"
#include "MapCell.h"
#include "CellWall.h"
#include "CellOpen.h"
#include <cassert>

// big four

Raycasting::Raycasting()
	: cellMap(),
	player(22.0f, 11.5f, -1.0f, 0.0f, 0.0f, 0.66f),
	buffer(SCREEN_WIDTH, SCREEN_HEIGHT),
	texture(),
	pixelSize(1)
{
	sAppName = "Raycasting";
}

Raycasting::~Raycasting()
{
	// do nothing (for now)
}

// pge functions

bool Raycasting::OnUserCreate()
{
	// called once at the start, so create things here

	cellMap.LoadMapFromCSV("world_2");

	olc::Sprite currTex;

	for (int texNum = 0; texNum < NUM_TEXTURES; texNum++)
	{
		switch (texNum)
		{
		case 0:
			currTex = olc::Sprite("resources/textures/eagle.png");
			break;

		case 1:
			currTex = olc::Sprite("resources/textures/redbrick.png");
			break;

		case 2:
			currTex = olc::Sprite("resources/textures/purplestone.png");
			break;

		case 3:
			currTex = olc::Sprite("resources/textures/greystone.png");
			break;

		case 4:
			currTex = olc::Sprite("resources/textures/bluestone.png");
			break;

		case 5:
			currTex = olc::Sprite("resources/textures/mossy.png");
			break;

		case 6:
			currTex = olc::Sprite("resources/textures/wood.png");
			break;

		case 7:
			currTex = olc::Sprite("resources/textures/colorstone.png");
			break;

		default:
			assert(false);
		}

		for (int x = 0; x < TEX_WIDTH; x++)
		{
			for (int y = 0; y < TEX_HEIGHT; y++)
			{
				texture[texNum][x][y] = currTex.GetPixel(x, y);
			}
		}
	}

	return true;
}

bool Raycasting::OnUserUpdate(float fElapsedTime)
{
	// called once per frame

	Clear(olc::BLACK);

	//// auto-adjust resolution based on framerate
	//if (fElapsedTime > TARGET_FRAME_TIME)
	//{
	//	pixelSize++;
	//}
	//else if (pixelSize > 1)
	//{
	//	pixelSize--;
	//}

	SmartFloorcasting();

	TexturedRaycasting();

	DrawSprite(0, 0, &buffer);

	MovePlayer(fElapsedTime);

	return true;
}

// raycasting functions

void Raycasting::PaintersFloorcasting()
{
	///////////////////
	// FLOOR CASTING //
	///////////////////

	for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
	{
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = SCREEN_WIDTH)
		float rayDirX0 = player.GetDir().GetX() - player.GetPlane().GetX();
		float rayDirY0 = player.GetDir().GetY() - player.GetPlane().GetY();
		float rayDirX1 = player.GetDir().GetX() + player.GetPlane().GetX();
		float rayDirY1 = player.GetDir().GetY() + player.GetPlane().GetY();

		// current y position compared to the center of the screen
		int p = y - SCREEN_HEIGHT / 2;

		// vertical position of camera
		float posZ = 0.5 * SCREEN_HEIGHT;

		// horizontal distance from the camera to the floor for the current row
		// 0.5 is the z position exactly in the middle between the floor and ceiling
		float rowDistance = posZ / p;

		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

		// real world coords of the leftmost column
		// this will be updated as we step to the right
		float floorX = player.GetPos().GetX() + rowDistance * rayDirX0;
		float floorY = player.GetPos().GetY() + rowDistance * rayDirY0;

		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			// the cell coord is simply gotten from the integer parts of floorX and floorY
			int cellX = static_cast<int>(floorX);
			int cellY = static_cast<int>(floorY);

			// get the texture coordinate from the fractional part
			int tx = static_cast<int>(TEX_WIDTH * (floorX - cellX)) & (TEX_WIDTH - 1);
			int ty = static_cast<int>(TEX_HEIGHT * (floorY - cellY)) & (TEX_HEIGHT - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			// choose the texture and draw the pixel
			olc::Pixel color;

			// floor
			color = texture[FLOOR_TEXTURE][tx][ty];
			color = color / 2; // make a bit darker
			Draw(x, y, color);

			// ceiling (symmetrical, at SCREEN_HEIGHT - y - 1 instead of y)
			color = texture[CEILING_TEXTURE][tx][ty];
			color = color / 2; // make a bit darker
			Draw(x, SCREEN_HEIGHT - y - 1, color);
		}
	}
}

void Raycasting::TexturedRaycasting()
{

	/////////////////////////////////
	// TEXTURED RAYCASTING (WALLS) //
	/////////////////////////////////

	for (int x = 0; x < SCREEN_WIDTH; x += pixelSize)
	{
		// calculate ray pos and dir
		float cameraX = 2.0f * x / static_cast<float>(SCREEN_WIDTH) - 1; // the x-coord in camera space
		Vector3 rayDir(player.GetDir().GetX() + player.GetPlane().GetX() * cameraX,
						player.GetDir().GetY() + player.GetPlane().GetY() * cameraX,
						0.0f);

		// which box of the map we're in
		int mapX = static_cast<int>(player.GetPos().GetX());
		int mapY = static_cast<int>(player.GetPos().GetY());

		// length of ray from current pos to next x- or y-side
		float sideDistX;
		float sideDistY;

		// length of ray from one x- or y-side to next x- or y-side
		float deltaDistX = (rayDir.GetX() == 0) ? 1e30f : std::abs(1 / rayDir.GetX());
		float deltaDistY = (rayDir.GetY() == 0) ? 1e30f : std::abs(1 / rayDir.GetY());
		float perpWallDist;

		// what direction to step in x- or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		int hit = 0; // was a wall hit?
		int side; // was a NS wall or EW wall hit?

		// calculate step and initial sideDist
		if (rayDir.GetX() < 0)
		{
			stepX = -1;
			sideDistX = (player.GetPos().GetX() - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - player.GetPos().GetX()) * deltaDistX;
		}

		if (rayDir.GetY() < 0)
		{
			stepY = -1;
			sideDistY = (player.GetPos().GetY() - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - player.GetPos().GetY()) * deltaDistY;
		}

		MapCell* pMapCell = nullptr;

		// perform DDA
		while (hit == 0)
		{
			// jump to next map square, either in x- or y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			// check if ray hit a wall
			pMapCell = cellMap.GetCell(mapX, mapY);
			assert(pMapCell != nullptr);
			if (pMapCell != nullptr && pMapCell->GetCellType() == MapCell::Type::Wall)
			{
				hit = 1;
			}
		}

		// calculate distance projected on camera direction
		// (euclidean distance would give fisheye effect!)
		if (side == 0)
		{
			perpWallDist = (sideDistX - deltaDistX);
		}
		else
		{
			perpWallDist = (sideDistY - deltaDistY);
		}

		// calculate height of line to draw on screen
		int lineHeight = static_cast<int>(SCREEN_HEIGHT / perpWallDist);

		// calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawStart < 0)
		{
			drawStart = 0;
		}

		int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT)
		{
			drawEnd = SCREEN_HEIGHT - 1;
		}

		// texturing calculations
		assert(pMapCell->GetCellType() == MapCell::Type::Wall);
		CellWall* pWall = static_cast<CellWall*>(pMapCell);
		int texNum = pWall->GetTexIndex() - 1; // 1 subtracted from it so texture 0 can be used!
		// TODO: fix so you don't have to do -1 at runtime

		// calculate the value of wallX
		float wallX; // where exactly the wall was hit
		if (side == 0)
		{
			wallX = player.GetPos().GetY() + perpWallDist * rayDir.GetY();
		}
		else
		{
			wallX = player.GetPos().GetX() + perpWallDist * rayDir.GetX();
		}
		wallX -= floor(wallX);

		// x-coordinate on the texture
		int texX = static_cast<int>(wallX * static_cast<float>(TEX_WIDTH));
		if (side == 0 && rayDir.GetX() > 0)
		{
			texX = TEX_WIDTH - texX - 1;
		}
		if (side == 1 && rayDir.GetY() < 0)
		{
			texX = TEX_WIDTH - texX - 1;
		}
		//texX = TEX_WIDTH - texX - 1;

		// how much to increase the texture coordinate per screen pixel
		float step = 1.0f * TEX_HEIGHT / lineHeight;

		// starting texture coordinate
		float texPos = (drawStart - SCREEN_HEIGHT / 2.0f + lineHeight / 2.0f) * step;
		for (int y = drawStart; y < drawEnd; y += pixelSize)
		{
			// cast the texture coordinate to integer and mask it with (TEX_HEIGHT - 1) in case of overflow
			int texY = static_cast<int>(texPos) & (TEX_HEIGHT - 1);
			texPos += step;
			olc::Pixel color = texture[texNum][texX][texY];

			// make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1)
			{
				color = color / 2;
			}

			//Draw(x, y, color);
			buffer.SetPixel(x, y, color);
		}

		/*

		//////////////////////////////////////
		// FLOOR CASTING (VERTICAL VERSION) //
		//////////////////////////////////////

		float floorXWall; // x position of the floor texel at the bottom of the wall
		float floorYWall; // y position of the floor texel at the bottom of the wall

		// 4 different wall directions possible
		if (side == 0 && rayDir.GetX() > 0)
		{
			floorXWall = static_cast<float>(mapX);
			floorYWall = mapY + wallX;
		}
		else if (side == 0 && rayDir.GetX() < 0)
		{
			floorXWall = mapX + 1.0f;
			floorYWall = mapY + wallX;
		}
		else if (side == 1 && rayDir.GetY() > 0)
		{
			floorXWall = mapX + wallX;
			floorYWall = static_cast<float>(mapY);
		}
		else
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0f;
		}
			
		float distWall = perpWallDist;
		float distPlayer = 0.0f;
		float currentDist;

		if (drawEnd < 0)
		{
			drawEnd = SCREEN_HEIGHT; // becomes < 0 when the integer overflows
		}

		// draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < SCREEN_HEIGHT; y++)
		{
			currentDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT); // you could make a small lookup table for this instead

			float weight = (currentDist - distPlayer) / (distWall - distPlayer);

			float currentFloorX = weight * floorXWall + (1.0f - weight) * player.GetPos().GetX();
			float currentFloorY = weight * floorYWall + (1.0f - weight) * player.GetPos().GetY();

			int floorTexX = static_cast<int>(currentFloorX * TEX_WIDTH) % TEX_WIDTH;
			int floorTexY = static_cast<int>(currentFloorY * TEX_HEIGHT) % TEX_HEIGHT;

			olc::Pixel color;

			pMapCell = cellMap.GetCell(static_cast<int>(currentFloorX), floor(currentFloorY));
			if (pMapCell->GetCellType() == MapCell::Type::Open)
			{
				CellOpen* pOpen = static_cast<CellOpen*>(pMapCell);

				// floor
				int floorTexNum = pOpen->GetFloorTexIndex() - 1;
				assert(floorTexNum >= 0);
				//color = texture[FLOOR_TEXTURE].GetPixel(floorTexX, floorTexY);
				color = texture[pOpen->GetFloorTexIndex() - 1].GetPixel(floorTexX, floorTexY); // TODO: fix so you don't have to do -1 at runtime
				Draw(x, y, color);
				//Draw(x, y, olc::CYAN);

				// ceiling (symmetrical)
				int ceilingTexNum = pOpen->GetCeilingTexIndex() - 1;
				assert(ceilingTexNum >= 0);
				//color = texture[CEILING_TEXTURE].GetPixel(floorTexX, floorTexY);
				color = texture[pOpen->GetCeilingTexIndex() - 1].GetPixel(floorTexX, floorTexY); 
				Draw(x, SCREEN_HEIGHT - y, color);
				//Draw(x, SCREEN_HEIGHT - y - 1, olc::CYAN);
			}
		}

		//*/
	}
}

void Raycasting::SmartFloorcasting()
{
	///////////////////////////////////////
	// FLOOR CASTING (HORIZONTAL, SMART) //
	///////////////////////////////////////

	for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y += pixelSize)
	{
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = SCREEN_WIDTH)
		Vector3 rayDirLeft = player.GetDir() - player.GetPlane();
		Vector3 rayDirRight = player.GetDir() + player.GetPlane();

		// current y position compared to the center of the screen
		int p = y - SCREEN_HEIGHT / 2;

		// vertical position of camera
		float posZ = 0.5f * SCREEN_HEIGHT;

		// horizontal distance from the camera to the floor for the current row
		// 0.5 is the z position exactly in the middle between the floor and ceiling
		float rowDistance = posZ / p;

		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		Vector3 floorStep = rowDistance * (rayDirRight - rayDirLeft) / SCREEN_WIDTH;

		// real world coords of the leftmost column
		// this will be updated as we step to the right
		Vector3 floorPos = player.GetPos() + rowDistance * rayDirLeft;

		for (int x = 0; x < SCREEN_WIDTH; x += pixelSize)
		{
			// the cell coord is simply gotten from the integer parts of floorX and floorY
			int cellX = static_cast<int>(floorPos.GetX());
			int cellY = static_cast<int>(floorPos.GetY());

			MapCell* pMapCell = cellMap.GetCell(cellX, cellY);

			if (pMapCell != nullptr && pMapCell->GetCellType() == MapCell::Type::Open)
			{
				CellOpen* pOpenCell = static_cast<CellOpen*>(pMapCell);

				// get the texture coordinate from the fractional part
				int tx = static_cast<int>(TEX_WIDTH * (floorPos.GetX() - cellX)) & (TEX_WIDTH - 1);
				int ty = static_cast<int>(TEX_HEIGHT * (floorPos.GetY() - cellY)) & (TEX_HEIGHT - 1);

				// choose the texture and draw the pixel
				olc::Pixel color;

				// floor
				//color = texture[FLOOR_TEXTURE].GetPixel(tx, ty);
				color = texture[pOpenCell->GetFloorTexIndex() - 1][tx][ty]; // TODO: fix so you don't have to do -1 at runtime
				color = color / 2; // make a bit darker
				//Draw(x, y, color);
				buffer.SetPixel(x, y, color);

				// ceiling (symmetrical, at SCREEN_HEIGHT - y - 1 instead of y)
				color = texture[pOpenCell->GetCeilingTexIndex() - 1][tx][ty]; // TODO: fix so you don't have to do -1 at runtime
				color = color / 2; // make a bit darker
				//Draw(x, SCREEN_HEIGHT - y - 1, color);
				buffer.SetPixel(x, SCREEN_HEIGHT - y - 1, color);
			}

			//floorPos += floorStep;
			floorPos.SetX(floorPos.GetX() + floorStep.GetX());
			floorPos.SetY(floorPos.GetY() + floorStep.GetY());
		}
	}
}

void Raycasting::MovePlayer(float fElapsedTime)
{
	// move forward if no wall in front of you
	if (GetKey(MOVE_FWD_KEY).bHeld)
	{
		player.MoveForward(fElapsedTime, &cellMap);
	}

	// move backward if no wall behind you
	if (GetKey(MOVE_BACK_KEY).bHeld)
	{
		player.MoveBackward(fElapsedTime, &cellMap);
	}

	// strafe left if no wall to your left
	if (GetKey(STRAFE_LEFT_KEY).bHeld)
	{
		player.StrafeLeft(fElapsedTime, &cellMap);
	}

	// strafe right if no wall to your right
	if (GetKey(STRAFE_RIGHT_KEY).bHeld)
	{
		player.StrafeRight(fElapsedTime, &cellMap);
	}

	// rotate to the left
	if (GetKey(ROTATE_LEFT_KEY).bHeld)
	{
		player.RotateLeft(fElapsedTime);
	}

	// rotate to the right
	if (GetKey(ROTATE_RIGHT_KEY).bHeld)
	{
		player.RotateRight(fElapsedTime);
	}
}
