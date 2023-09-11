#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "olcPixelGameEngine.h"
#include "Variables.h"

#include "Player.h"
#include "RaycastMap.h"

class Raycasting : public olc::PixelGameEngine
{
public:
	Raycasting();
	Raycasting(const Raycasting& r) = delete;
	Raycasting& operator=(const Raycasting& r) = delete;
	virtual ~Raycasting();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	void PaintersFloorcasting();
	void TexturedRaycasting();
	void SmartFloorcasting();
	void MovePlayer(float fElapsedTime);

private:
	RaycastMap cellMap;
	Player player;
	//uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]; // y-coordinate first because it works per scanline
	olc::Sprite buffer;
	float zbuff[SCREEN_WIDTH];
	olc::Pixel texture[NUM_TEXTURES][TEX_WIDTH][TEX_HEIGHT];

	int pixelSize;
};

#endif