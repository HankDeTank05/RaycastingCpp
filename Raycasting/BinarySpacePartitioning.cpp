#include "BinarySpacePartitioning.h"

#include <cassert>

#include "Vector4.h"
#include "Matrix44.h"
#include "LineDef.h"

BinarySpacePartitioning::BinarySpacePartitioning()
{
	sAppName = "Binary Space Partitioning";
}

BinarySpacePartitioning::~BinarySpacePartitioning()
{
	// do nothing (for now)
}

bool BinarySpacePartitioning::OnUserCreate()
{
	// make sure the screen width is an even number so our lookup table works
	assert(SCREEN_WIDTH % 2 == 0);

	// create sector map
	worldMap.AddLineDef(LineDef(-2.0f, -1.0f, 2.0f, -5.0f));

	// set player pos and dir
	pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	dir = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	plane = Vector4(0.66f, 0.0f, 0.0f, 0.0f);

	// create lookup table for converting angles to screen-x
	for (int x = 0; x < angleToX.size(); x++)
	{
		float cameraX = 2.0f * x / static_cast<float>(SCREEN_WIDTH) - 1; // x-coord in camera space
		Vector4 rayDir(dir.GetX() + plane.GetX() * cameraX,
			0.0f,
			dir.GetZ() + plane.GetZ() * cameraX,
			0.0f);

		rayDir.normalize();

		angleToX[x] = rayDir.dot(dir);
	}

	return true;
}

bool BinarySpacePartitioning::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	std::vector<LineDef> lineDefs = worldMap.GetLineDefs();

	for (int ld = 0; ld < lineDefs.size(); ld++)
	{
		LineDef currLineDef = lineDefs[ld];
		if (dir.dot(currLineDef.GetNormal()) < 0.0f)
		{
			// draw the linedef from the leftmost column to the rightmost column

			Vector4 vLeft = currLineDef.GetP0() - pos;
			Vector4 vRight = currLineDef.GetP1() - pos;

			//vLeft.normalize();
			//vRight.normalize();

			float leftDot = vLeft.getNorm().dot(dir);
			float rightDot = vRight.getNorm().dot(dir);

			Vector4 dirLeft = Vector4(0.0f, 1.0f, 0.0f, 0.0f).cross(dir);
			dirLeft.normalize();

			bool leftEndIsScreenLeft = dirLeft.dot(vLeft.getNorm()) > 0.0f;
			bool rightEndIsScreenLeft = dirLeft.dot(vRight.getNorm()) > 0.0f;

			int leftX = BruteForceSearchForX(leftDot, leftEndIsScreenLeft);
			int rightX = BruteForceSearchForX(rightDot, rightEndIsScreenLeft);

			/*
			* how to get the wall point distance from the camera "plane"
			* 1. project (wall point - player pos) onto the camera "plane" vector
			* 2. subtract the projected vector from the original (wall point - player pos) vector
			* 3. the above step returns a vector perpendicular to the projection. get its magnitude
			*/

			Vector4 vLeftProjection = (vLeft.dot(plane) / plane.dot(plane)) * plane;
			Vector4 vRightProjection = (vRight.dot(plane) / plane.dot(plane)) * plane;

			Vector4 vLeftPerp = vLeft - vLeftProjection;
			Vector4 vRightPerp = vRight - vRightProjection;

			// TODO: you might be able to use magSqr() here to avoid a sqaure-root...
			float leftPerpDist = vLeftPerp.mag();
			float rightPerpDist = vRightPerp.mag();

			for (int x = leftX; x < rightX; x++)
			{
				float cameraX = 2.0f * x / static_cast<float>(SCREEN_WIDTH) - 1; // x-coord in camera space
				Vector4 rayDir(dir.GetX() + plane.GetX() * cameraX,
					0.0f,
					dir.GetZ() + plane.GetZ() * cameraX,
					0.0f);

				float perpWallDist;

				/* 
				// the following was my attempt to interpolate between the left dist and right dist, but
				// resulted in a fisheye-like effect, so we gotta do it the hard way...

				float amountAcrossWall = static_cast<float>(x - leftX) / static_cast<float>(rightX - leftX);
				perpWallDist = amountAcrossWall * (rightPerpDist - leftPerpDist) + leftPerpDist;
				//*/

				//*
				Vector4 lineDefVect = currLineDef.GetP1() - currLineDef.GetP0();
				// p1 = pos
				// v1 = rayDir
				// p2 = currLineDef.GetP0()
				// v2 = lineDefVect
				float a = rayDir.dot(rayDir);
				float b = rayDir.dot(lineDefVect);
				float c = lineDefVect.dot(lineDefVect);
				float d = (pos - currLineDef.GetP0()).dot(rayDir);
				float e = (pos - currLineDef.GetP0()).dot(lineDefVect);
				assert(b * b - a * c != 0.0f);
				float t1 = (c * d - b * e) / (b * b - a * c);
				float t2 = (d * b - a * e) / (b * b - a * c);
				Vector4 c1 = pos + t1 * rayDir;
				Vector4 c2 = currLineDef.GetP0() + t2 * lineDefVect;
				//assert(c1 == c2);
				Vector4 toProject = c1 - pos;
				Vector4 projected = (toProject.dot(plane) / plane.dot(plane)) * plane;
				Vector4 perpendicular = toProject - projected;
				perpWallDist = perpendicular.mag();
				//*/

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

				olc::Pixel color = olc::WHITE;

				float colorAdjustment = abs(currLineDef.GetNormal().dot(dir));

				DrawLine(x, drawStart, x, drawEnd, color / colorAdjustment);
			}
		}
	}

	if (GetKey(ROTATE_LEFT_KEY).bHeld)
	{
		Matrix44 rotMat = Matrix44::RotY(MOVE_SPEED * fElapsedTime);
		dir = rotMat * dir;
		plane = rotMat * plane;
	}

	if (GetKey(ROTATE_RIGHT_KEY).bHeld)
	{
		Matrix44 rotMat = Matrix44::RotY(-MOVE_SPEED * fElapsedTime);
		dir = rotMat * dir;
		plane = rotMat * plane;
	}

	return true;
}

int BinarySpacePartitioning::BruteForceSearchForX(float dotProduct, bool leftSideOfScreen)
{
	// case: no match (outside screen bounds)
	if (dotProduct < angleToX[0])
	{
		// case: left side of screen
		if (leftSideOfScreen)
		{
			return 0;
		}

		// case: right side of screen
		else
		{
			return SCREEN_WIDTH - 1;
		}
	}

	for (int x = 0; x < angleToX.size() - 1; x++)
	{
		// case: exact match
		if (dotProduct == angleToX[x])
		{
			// case: left side of screen
			if (leftSideOfScreen)
			{
				return x;
			}

			// case: right side of screen
			else
			{
				return SCREEN_WIDTH - x;
			}
		}

		// case: imperfect match (between current and next, exclusive)
		else if (angleToX[x] < dotProduct && dotProduct < angleToX[x + 1])
		{
			// case: left side of screen
			if (leftSideOfScreen)
			{
				return x;
			}

			// case: right side of screen
			else
			{
				return SCREEN_WIDTH - x;
			}
		}
	}

}

//int BinarySpacePartitioning::BruteForceSearchForX(float angle)
//{
//	// case: angle is 
//	int screenX = 0;
//	while (screenX < angleToX.size() - 1)
//	{
//		// case: angle matches current x-angle exactly
//		if (angle == angleToX[screenX])
//		{
//			return screenX;
//		}
//
//		// case: angle is in between current x-angle and next x-angle
//		else if (angleToX[screenX] < angle && angle < angleToX[screenX + 1])
//		{
//			return screenX;
//		}
//
//		screenX++;
//	}
//
//}
//
//int BinarySpacePartitioning::BinSearchForX(float angle)
//{
//	return BinSearchForX(angle, 0, angleToX.size() - 1);
//}
//
//int BinarySpacePartitioning::BinSearchForX(float angle, int left, int right)
//{
//	if (left == right - 1 || left == right)
//	{
//		return left;
//	}
//	int mid = (left + right) / 2;
//
//	// make sure indices are valid
//	assert(left <= mid);
//	assert(mid <= right);
//	assert(left < right);
//	assert(0 <= left);
//	assert(right < angleToX.size());
//
//	// NOTE: angleToX IS REVERSE SORTED!
//	// THIS MEANS...
//	// LEFT IS LARGER, RIGHT IS SMALLER
//
//	// case: angle is on left (larger)
//	if (angleToX[left] >= angle && angle > angleToX[mid])
//	{
//		// case: imperfect match
//		if (left == mid - 1 && angle != angleToX[left])
//		{
//			return left;
//		}
//		// case: still searching
//		return BinSearchForX(angle, left, mid - 1);
//	}
//
//	// case: angle is on right (smaller)
//	else if (angleToX[mid] >= angle && angle >= angleToX[right])
//	{
//		// case: imperfect match
//		if (mid == right - 1)
//		{
//			if (angle == angleToX[mid] || angle > angleToX[right])
//			{
//				return mid;
//			}
//			else if (angle == angleToX[right])
//			{
//				return right;
//			}
//		}
//		// case: still searching
//		return BinSearchForX(angle, mid, right);
//	}
//
//	// base case
//	else
//	{
//		return SCREEN_WIDTH;
//	}
//
//}
