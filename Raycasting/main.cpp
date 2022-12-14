#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

#include "Variables.h"
#include <vector>

class Raycasting : public olc::PixelGameEngine
{
public:
	Raycasting()
	{
		sAppName = "Raycasting";
	}

private:
	int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
		{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
		{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
		{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
		{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
		{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
		{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
		{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
		{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
		{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
		{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
		{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
		{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
		{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
		{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
		{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
		{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
		{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
		{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
		{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
		{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
		{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
	};

	double posX, posY; // x and y start position
	double dirX, dirY; // initial direction vector
	double planeX, planeY; // the 2d raycaster version of camera plane

	//uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]; // y-coordinate first because it works per scanline
	olc::Sprite texture[NUM_TEXTURES];

public:
	bool OnUserCreate() override
	{
		// called once at the start, so create things here

		// x and y start position
		posX = 22; posY = 11.5;

		// initial direction vector
		dirX = -1; dirY = 0;

		// the 2d raycaster version of a camera plane
		planeX = 0; planeY = 0.66;


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

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

		Clear(olc::BLACK);

		  ///////////////////
		 // FLOOR CASTING //
		///////////////////

		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			// rayDir for leftmost ray (x = 0) and rightmost ray (x = SCREEN_WIDTH)
			float rayDirX0 = dirX - planeX;
			float rayDirY0 = dirY - planeY;
			float rayDirX1 = dirX + planeX;
			float rayDirY1 = dirY + planeY;

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
			float floorX = posX + rowDistance * rayDirX0;
			float floorY = posY + rowDistance * rayDirY0;

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

		  /////////////////////////////////
		 // TEXTURED RAYCASTING (WALLS) //
		/////////////////////////////////

		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			// calculate ray pos and dir
			double cameraX = 2.0 * x / (double)SCREEN_WIDTH - 1; // the x-coord in camera space
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			// which box of the map we're in
			int mapX = (int)posX;
			int mapY = (int)posY;

			// length of ray from current pos to next x- or y-side
			double sideDistX;
			double sideDistY;

			// length of ray from one x- or y-side to next x- or y-side
			double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
			double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
			double perpWallDist;

			// what direction to step in x- or y-direction (either +1 or -1)
			int stepX;
			int stepY;
			
			int hit = 0; // was a wall hit?
			int side; // was a NS wall or EW wall hit?

			// calculate step and initial sideDist
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}

			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}

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
				if (worldMap[mapX][mapY] > 0)
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
			int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

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
			int texNum = worldMap[mapX][mapY] - 1; // 1 subtracted from it so texture 0 can be used!

			// calculate the value of wallX
			double wallX; // where exactly the wall was hit
			if (side == 0)
			{
				wallX = posY + perpWallDist * rayDirY;
			}
			else
			{
				wallX = posX + perpWallDist * rayDirX;
			}
			wallX -= floor(wallX);
			
			// x-coordinate on the texture
			int texX = (int)(wallX * (double)(TEX_WIDTH));
			if (side == 0 && rayDirX > 0)
			{
				texX = TEX_WIDTH - texX - 1;
			}
			if (side == 1 && rayDirY < 0)
			{
				texX = TEX_WIDTH - texX - 1;
			}
			//texX = TEX_WIDTH - texX - 1;

			// how much to increase the texture coordinate per screen pixel
			double step = 1.0 * TEX_HEIGHT / lineHeight;
			
			// starting texture coordinate
			double texPos = (drawStart - SCREEN_HEIGHT / 2.0 + lineHeight / 2.0) * step;
			for (int y = drawStart; y < drawEnd; y++)
			{
				// cast the texture coordinate to integer and mask it with (TEX_HEIGHT - 1) in case of overflow
				int texY = (int)(texPos) & (TEX_HEIGHT - 1);
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
		}

		// speed modifiers
		double moveSpeed = fElapsedTime * MOVE_SPEED;
		double rotSpeed = fElapsedTime * ROT_SPEED;

		// move forward if no wall in front of you
		if (GetKey(MOVE_FWD_KEY).bHeld)
		{
			if (worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == false)
			{
				posX += dirX * moveSpeed;
			}
			if (worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == false)
			{
				posY += dirY * moveSpeed;
			}
		}

		// move backward if no wall behind you
		if (GetKey(MOVE_BACK_KEY).bHeld)
		{
			if (worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == false)
			{
				posX -= dirX * moveSpeed;
			}
			if (worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == false)
			{
				posY -= dirY * moveSpeed;
			}
		}

		//// strafe right if no wall to your right
		//if (GetKey(STRAFE_RIGHT_KEY).bHeld)
		//{
		//	if (worldMap[(int)(posX + dirY * moveSpeed)][(int)(posY)] == false)
		//	{
		//		posX += dirY * moveSpeed;
		//	}
		//	if (worldMap[(int)(posX)][(int)(posY - dirX * moveSpeed)] == false)
		//	{
		//		posY -= dirX * moveSpeed;
		//	}
		//}

		//// strafe left if no wall to your left
		//if (GetKey(STRAFE_LEFT_KEY).bHeld)
		//{
		//	if (worldMap[(int)(posX - dirY * moveSpeed)][(int)(posY)] == false)
		//	{
		//		posX -= dirY * moveSpeed;
		//	}
		//	if (worldMap[(int)(posX)][(int)(posY + dirX * moveSpeed)] == false)
		//	{
		//		posY += dirX * moveSpeed;
		//	}
		//}

		// rotate to the right
		if (GetKey(STRAFE_RIGHT_KEY).bHeld)
		{
			// both camera direction and camera plane must be rotated
			double oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}

		// rotate to the left
		if (GetKey(STRAFE_LEFT_KEY).bHeld)
		{
			// both camera direction and camera plane must be rotated
			double oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}

		return true;
	}
};

int main()
{
	Raycasting game;
	if (game.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE_WIDTH, SCREEN_SCALE_HEIGHT))
	{
		game.Start();
	}

	return 0;
}