#ifndef PLAYER_H
#define PLAYER_H

#include "Vector3.h"

class Player
{
public:
	// big four

	Player() = delete;
	Player(const Player& p) = delete;
	Player& operator=(const Player& p);
	virtual ~Player();

	// specialized constructor
	Player(float posX, float posY,
		float dirX, float dirY,
		float planeX, float planeY);

	// movement functions

	void MoveForward(float fElapsedTime);
	void MoveBackward(float fElapsedTime);

	void StrafeLeft(float fElapsedTime);
	void StrafeRight(float fElapsedTime);

	void RotateLeft(float fElapsedTime);
	void RotateRight(float fElapsedTime);

public: // accessors
	Vector3 GetPos();
	Vector3 GetDir();
	Vector3 GetPlane();

	float GetMoveSpeed();
	float GetRotSpeed();

private:
	Vector3 pos;
	Vector3 dir;
	Vector3 plane; // the 2d raycaster version of a camera plane
	float moveSpeed;
	float rotSpeed;

private: // prevent implicit conversions
	Player(int, int, int, int, int, int);
	Player(double, double, double, double, double, double);
};

#endif