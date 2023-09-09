#include "Player.h"

#include "Variables.h"

#include "Matrix33.h"
#include "RaycastMap.h"
#include "MapCell.h"

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

void Player::MoveForward(float fElapsedTime, RaycastMap* pMap)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	int nextX = static_cast<int>(pos.GetX() + dir.GetX() * moveSpeed);
	int nextY = static_cast<int>(pos.GetY() + dir.GetY() * moveSpeed);

	MapCell* pCell = pMap->GetCell(nextX, pos.GetY());
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetX(pos.GetX() + dir.GetX() * moveSpeed);
	}

	pCell = pMap->GetCell(pos.GetX(), nextY);
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetY(pos.GetY() + dir.GetY() * moveSpeed);
	}
}

void Player::MoveBackward(float fElapsedTime, RaycastMap* pMap)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	int nextX = static_cast<int>(pos.GetX() - dir.GetX() * moveSpeed);
	int nextY = static_cast<int>(pos.GetY() - dir.GetY() * moveSpeed);

	MapCell* pCell = pMap->GetCell(nextX, pos.GetY());
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetX(pos.GetX() - dir.GetX() * moveSpeed);
	}

	pCell = pMap->GetCell(pos.GetX(), nextY);
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetY(pos.GetY() - dir.GetY() * moveSpeed);
	}
}

void Player::StrafeLeft(float fElapsedTime, RaycastMap* pMap)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	int nextX = static_cast<int>(pos.GetX() - dir.GetY() * moveSpeed);
	int nextY = static_cast<int>(pos.GetY() + dir.GetX() * moveSpeed);

	MapCell* pCell = pMap->GetCell(nextX, pos.GetY());
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetX(pos.GetX() - dir.GetY() * moveSpeed);
	}

	pCell = pMap->GetCell(pos.GetX(), nextY);
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetY(pos.GetY() + dir.GetX() * moveSpeed);
	}
}

void Player::StrafeRight(float fElapsedTime, RaycastMap* pMap)
{
	// speed modifiers
	float moveSpeed = fElapsedTime * MOVE_SPEED;

	int nextX = static_cast<int>(pos.GetX() + dir.GetY() * moveSpeed);
	int nextY = static_cast<int>(pos.GetY() - dir.GetX() * moveSpeed);

	MapCell* pCell = pMap->GetCell(nextX, pos.GetY());
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetX(pos.GetX() + dir.GetY() * moveSpeed);
	}

	pCell = pMap->GetCell(pos.GetX(), nextY);
	if (pCell != nullptr && pCell->GetCellType() != MapCell::Type::Wall)
	{
		pos.SetY(pos.GetY() - dir.GetX() * moveSpeed);
	}
}

void Player::RotateLeft(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	float cos = cosf(rotSpeed);
	float sin = sinf(rotSpeed);

	Matrix33 rotMat(cos, -sin, 0.0f,
					sin, cos, 0.0f,
					0.0f, 0.0f, 1.0f);

	// both camera direction and camera plane must be rotated
	dir = rotMat * dir;
	plane = rotMat * plane;
}

void Player::RotateRight(float fElapsedTime)
{
	// speed modifiers
	float rotSpeed = fElapsedTime * ROT_SPEED;

	float cos = cosf(-rotSpeed);
	float sin = sinf(-rotSpeed);

	Matrix33 rotMat(cos, -sin, 0.0f,
					sin, cos, 0.0f,
					0.0f, 0.0f, 1.0f);

	// both camera direction and camera plane must be rotated
	dir = rotMat * dir;
	plane = rotMat * plane;
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
