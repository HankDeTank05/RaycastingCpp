#include "Raycasting.h"

#include "Player.h"
#include "MapCell.h"
#include "CellWall.h"
#include "CellOpen.h"
#include <cassert>

// big four

Raycasting::Raycasting()
	: player(22.0f, 11.5f, -1.0f, 0.0f, 0.0f, 0.66f)
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

	cellMap.LoadMapFromText("world_1");

	texture[0] = olc::Sprite("resources/eagle.png");
	texture[1] = olc::Sprite("resources/redbrick.png");
	texture[2] = olc::Sprite("resources/purplestone.png");
	texture[3] = olc::Sprite("resources/greystone.png");
	texture[4] = olc::Sprite("resources/bluestone.png");
	texture[5] = olc::Sprite("resources/mossy.png");
	texture[6] = olc::Sprite("resources/wood.png");
	texture[7] = olc::Sprite("resources/colorstone.png");

	return true;
}

bool Raycasting::OnUserUpdate(float fElapsedTime)
{
	// called once per frame

	Clear(olc::BLACK);

	TexturedRaycasting(Raycasting::FloorCasting::Vertical);

	MovePlayer(fElapsedTime);

	return true;
}

// raycasting functions

void Raycasting::PaintersFloorCasting()
{
	///////////////////
	// FLOOR CASTING //
	///////////////////

	for (int y = 0; y < SCREEN_HEIGHT; y++)
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
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);

			// get the texture coordinate from the fractional part
			int tx = (int)(TEX_WIDTH * (floorX - cellX)) & (TEX_WIDTH - 1);
			int ty = (int)(TEX_HEIGHT * (floorY - cellY)) & (TEX_HEIGHT - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			// choose the texture and draw the pixel
			olc::Pixel color;

			// floor
			color = texture[FLOOR_TEXTURE].GetPixel(tx, ty);
			color = color / 2; // make a bit darker
			Draw(x, y, color);

			// ceiling (symmetrical, at SCREEN_HEIGHT - y - 1 instead of y)
			color = texture[CEILING_TEXTURE].GetPixel(tx, ty);
			color = color / 2; // make a bit darker
			Draw(x, SCREEN_HEIGHT - y - 1, color);
		}
	}
}

void Raycasting::TexturedRaycasting(Raycasting::FloorCasting algo)
{
	// TODO: replace with strategy pattern
	if (algo == Raycasting::FloorCasting::Painters)
	{
		PaintersFloorCasting();
	}
	else if (algo == Raycasting::FloorCasting::None)
	{
		// floor
		FillRect(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, olc::GREY);

		// ceiling
		FillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, olc::DARK_GREY);
	}

	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		/////////////////////////////////
		// TEXTURED RAYCASTING (WALLS) //
		/////////////////////////////////

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
			if (pMapCell->GetCellType() == MapCell::Type::Wall)
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
		for (int y = drawStart; y < drawEnd; y++)
		{
			// cast the texture coordinate to integer and mask it with (TEX_HEIGHT - 1) in case of overflow
			int texY = static_cast<int>(texPos) & (TEX_HEIGHT - 1);
			texPos += step;
			olc::Pixel color = texture[texNum].GetPixel(texX, texY);

			// make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1)
			{
				color = color / 2;
			}

			//buffer[y][x] = color;
			Draw(x, y, color);
		}

		// TODO: replace with strategy pattern
		if (algo == Raycasting::FloorCasting::Vertical)
		{
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

				// floor
				color = texture[FLOOR_TEXTURE].GetPixel(floorTexX, floorTexY);
				Draw(x, y, color);

				// ceiling (symmetrical)
				color = texture[CEILING_TEXTURE].GetPixel(floorTexX, floorTexY);
				Draw(x, SCREEN_HEIGHT - y, color);
			}
		}
	}
}

void Raycasting::MovePlayer(float fElapsedTime)
{
	// move forward if no wall in front of you
	if (GetKey(MOVE_FWD_KEY).bHeld)
	{
		player.MoveForward(fElapsedTime);
	}

	// move backward if no wall behind you
	if (GetKey(MOVE_BACK_KEY).bHeld)
	{
		player.MoveBackward(fElapsedTime);
	}

	// strafe left if no wall to your left
	if (GetKey(STRAFE_LEFT_KEY).bHeld)
	{
		player.StrafeLeft(fElapsedTime);
	}

	// strafe right if no wall to your right
	if (GetKey(STRAFE_RIGHT_KEY).bHeld)
	{
		player.StrafeRight(fElapsedTime);
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
