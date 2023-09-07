#include "Player.h"

#include "Variables.h"

Player::Player(float _posX, float _posY, float _dirX, float _dirY, float _planeX, float _planeY)
	//: posX(_posX), posY(_posY),
	//dirX(_dirX), dirY(_dirY),
	//planeX(_planeX), planeY(_planeY),
	: pos(_posX, _posY, 1.0f),
	dir(_dirX, _dirY, 0.0f),
	plane(_planeX, _planeY, 0.0f),
	moveSpeed(MOVE_SPEED),
	rotSpeed(ROT_SPEED)

{
	// do nothing
}

Player& Player::operator=(const Player& p)
{
	//posX = p.posX; posY = p.posY;
	//dirX = p.dirX; dirY = p.dirY;
	//planeX = p.planeX; planeY = p.planeY;
	pos = p.pos;
	dir = p.dir;
	plane = p.plane;

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
	if (WORLD_2[static_cast<int>(pos.GetX() + dir.GetX() * moveSpeed)][static_cast<int>(pos.GetY())] == false)
	{
		pos.SetX(pos.GetX() + dir.GetX() * moveSpeed);
	}
	if (WORLD_2[static_cast<int>(pos.GetX())][static_cast<int>(pos.GetY() + dir.GetY() * moveSpeed)] == false)
	{
		pos.SetY(pos.GetY() + dir.GetY() * moveSpeed);
	}
}

void Player::MoveBackward(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(pos.GetX() - dir.GetX() * moveSpeed)][static_cast<int>(pos.GetY())] == false)
	{
		pos.SetX(pos.GetX() - dir.GetX() * moveSpeed);
	}
	if (WORLD_2[static_cast<int>(pos.GetX())][static_cast<int>(pos.GetY() - dir.GetY() * moveSpeed)] == false)
	{
		pos.SetY(pos.GetY() - dir.GetY() * moveSpeed);
	}
}

void Player::StrafeLeft(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(pos.GetX() - dir.GetY() * moveSpeed)][static_cast<int>(pos.GetY())] == false)
	{
		pos.SetX(pos.GetX() - dir.GetY() * moveSpeed);
	}
	if (WORLD_2[static_cast<int>(pos.GetX())][static_cast<int>(pos.GetY() + dir.GetX() * moveSpeed)] == false)
	{
		pos.SetY(pos.GetY() + dir.GetX() * moveSpeed);
	}
}

void Player::StrafeRight(float fElapsedTime)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	// TODO: replace constant map with map ref
	if (WORLD_2[static_cast<int>(pos.GetX() + dir.GetY() * moveSpeed)][static_cast<int>(pos.GetY())] == false)
	{
		pos.SetX(pos.GetX() + dir.GetY() * moveSpeed);
	}
	if (WORLD_2[static_cast<int>(pos.GetX())][static_cast<int>(pos.GetY() - dir.GetX() * moveSpeed)] == false)
	{
		pos.SetY(pos.GetY() - dir.GetX() * moveSpeed);
	}
}

void Player::RotateLeft(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	// both camera direction and camera plane must be rotated
	float oldDirX = dir.GetX();
	dir.SetX(dir.GetX() * cosf(rotSpeed) - dir.GetY() * sinf(rotSpeed));
	dir.SetY(oldDirX * sinf(rotSpeed) + dir.GetY() * cosf(rotSpeed));
	float oldPlaneX = plane.GetX();
	plane.SetX(plane.GetX() * cosf(rotSpeed) - plane.GetY() * sinf(rotSpeed));
	plane.SetY(oldPlaneX * sinf(rotSpeed) + plane.GetY() * cosf(rotSpeed));
}

void Player::RotateRight(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	// both camera direction and camera plane must be rotated
	float oldDirX = dir.GetX();
	dir.SetX(dir.GetX() * cosf(-rotSpeed) - dir.GetY() * sinf(-rotSpeed));
	dir.SetY(oldDirX * sinf(-rotSpeed) + dir.GetY() * cosf(-rotSpeed));
	float oldPlaneX = plane.GetX();
	plane.SetX(plane.GetX() * cosf(-rotSpeed) - plane.GetY() * sinf(-rotSpeed));
	plane.SetY(oldPlaneX * sinf(-rotSpeed) + plane.GetY() * cosf(-rotSpeed));
}

Vector3 Player::GetPos()
{
	return pos;
}

Vector3 Player::GetDir()
{
	return dir;
}

Vector3 Player::GetPlane()
{
	return plane;
}

float Player::GetMoveSpeed()
{
	return moveSpeed;
}

float Player::GetRotSpeed()
{
	return rotSpeed;
}
