#include "Player.h"

#include "Variables.h"

Player::Player(float _posX, float _posY, float _dirX, float _dirY, float _planeX, float _planeY)
	: posX(_posX), posY(_posY),
	dirX(_dirX), dirY(_dirY),
	planeX(_planeX), planeY(_planeY),
	moveSpeed(MOVE_SPEED),
	rotSpeed(ROT_SPEED)

{
	// do nothing
}

Player& Player::operator=(const Player& p)
{
	posX = p.posX; posY = p.posY;
	dirX = p.dirX; dirY = p.dirY;
	planeX = p.planeX; planeY = p.planeY;

	moveSpeed = p.moveSpeed;
	rotSpeed = p.rotSpeed;

	return *this;
}

Player::~Player()
{
	// do nothing
}

void Player::MoveForward(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;
	
	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(posX + dirX * moveSpeed)][static_cast<int>(posY)] == false)
	{
		posX += dirX * moveSpeed;
	}
	if (WORLD_2[static_cast<int>(posX)][static_cast<int>(posY + dirY * moveSpeed)] == false)
	{
		posY += dirY * moveSpeed;
	}
}

void Player::MoveBackward(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(posX - dirX * moveSpeed)][static_cast<int>(posY)] == false)
	{
		posX -= dirX * moveSpeed;
	}
	if (WORLD_2[static_cast<int>(posX)][static_cast<int>(posY - dirY * moveSpeed)] == false)
	{
		posY -= dirY * moveSpeed;
	}
}

void Player::StrafeLeft(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(posX - dirY * moveSpeed)][static_cast<int>(posY)] == false)
	{
		posX -= dirY * moveSpeed;
	}
	if (WORLD_2[static_cast<int>(posX)][static_cast<int>(posY + dirX * moveSpeed)] == false)
	{
		posY += dirX * moveSpeed;
	}
}

void Player::StrafeRight(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(posX + dirY * moveSpeed)][static_cast<int>(posY)] == false)
	{
		posX += dirY * moveSpeed;
	}
	if (WORLD_2[static_cast<int>(posX)][static_cast<int>(posY - dirX * moveSpeed)] == false)
	{
		posY -= dirX * moveSpeed;
	}
}

void Player::RotateLeft(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	// both camera direction and camera plane must be rotated
	float oldDirX = dirX;
	dirX = dirX * cosf(rotSpeed) - dirY * sinf(rotSpeed);
	dirY = oldDirX * sinf(rotSpeed) + dirY * cosf(rotSpeed);
	float oldPlaneX = planeX;
	planeX = planeX * cosf(rotSpeed) - planeY * sinf(rotSpeed);
	planeY = oldPlaneX * sinf(rotSpeed) + planeY * cosf(rotSpeed);
}

void Player::RotateRight(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	// both camera direction and camera plane must be rotated
	float oldDirX = dirX;
	dirX = dirX * cosf(-rotSpeed) - dirY * sinf(-rotSpeed);
	dirY = oldDirX * sinf(-rotSpeed) + dirY * cosf(-rotSpeed);
	float oldPlaneX = planeX;
	planeX = planeX * cosf(-rotSpeed) - planeY * sinf(-rotSpeed);
	planeY = oldPlaneX * sinf(-rotSpeed) + planeY * cosf(-rotSpeed);
}

float Player::GetPosX()
{
	return posX;
}

float Player::GetPosY()
{
	return posY;
}

float Player::GetDirX()
{
	return dirX;
}

float Player::GetDirY()
{
	return dirY;
}

float Player::GetPlaneX()
{
	return planeX;
}

float Player::GetPlaneY()
{
	return planeY;
}

float Player::GetMoveSpeed()
{
	return moveSpeed;
}

float Player::GetRotSpeed()
{
	return rotSpeed;
}
