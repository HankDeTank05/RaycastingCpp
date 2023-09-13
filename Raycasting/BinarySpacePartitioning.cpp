#include "BinarySpacePartitioning.h"

#include <cassert>

#include "Vector4.h"
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
	// math checks before we get going

	Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

	assert(xAxis.cross(yAxis) == zAxis);
	assert(zAxis.cross(xAxis) == yAxis);
	assert(yAxis.cross(zAxis) == xAxis);

	// create sector map
	worldMap.AddLineDef(LineDef(-5.0f, -10.0f, 5.0f, -10.0f));

	// set player pos and dir
	pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	dir = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	plane = Vector4(0.66f, 0.0f, 0.0f, 0.0f);

	// create lookup table for converting angles to screen-x
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		float cameraX = 2.0f * x / static_cast<float>(SCREEN_WIDTH) - 1; // x-coord in camera space
		Vector4 rayDir(dir.GetX() + plane.GetX() * cameraX,
			0.0f,
			dir.GetZ() + plane.GetZ() * cameraX,
			0.0f);

		rayDir.normalize();

		Vector4 comparisonAngle = (pos + dir - plane) - pos;
		comparisonAngle.normalize();

		float angle = rayDir.dot(comparisonAngle);

		//assert(angleToX.count(angle) == 0);
		//angleToX.insert(std::pair<float, int>(angle, x));
		assert(x < angleToX.size());
		angleToX[x] = angle;
	}

	return true;
}

bool BinarySpacePartitioning::OnUserUpdate(float fElapsedTime)
{
	std::vector<LineDef> lineDefs = worldMap.GetLineDefs();

	for (int ld = 0; ld < lineDefs.size(); ld++)
	{
		LineDef currLineDef = lineDefs[ld];
		if (dir.dot(currLineDef.GetNormal()) < 0.0f)
		{
			// draw the linedef from the leftmost column to the rightmost column

			Vector4 comparisonAngle = (pos + dir - plane) - pos;
			comparisonAngle.normalize();

			Vector4 dirToLeft = (currLineDef.GetP0() - pos);
			Vector4 dirToRight = (currLineDef.GetP1() - pos);

			dirToLeft.normalize();
			dirToRight.normalize();

			float leftAngle = dirToLeft.dot(comparisonAngle);
			float rightAngle = dirToRight.dot(comparisonAngle);

			// binary search for the proper column to draw
			int leftX = BinSearchForX(leftAngle);
			int rightX = BinSearchForX(rightAngle);

			for (int x = leftX; x < rightX; x++)
			{
				float cameraX = 2.0f * x / static_cast<float>(SCREEN_WIDTH) - 1; // x-coord in camera space
				Vector4 rayDir(dir.GetX() + plane.GetX() * cameraX,
					0.0f,
					dir.GetZ() + plane.GetZ() * cameraX,
					0.0f);

				float numer = (pos - currLineDef.GetP0()).dot(currLineDef.GetNormal());
				float denom = currLineDef.GetNormal().magSqr();

				Vector4 projOnNormal = (numer / denom) * currLineDef.GetNormal();

				float distance = abs(projOnNormal.mag());

				int lineHeight = static_cast<int>(SCREEN_HEIGHT / distance);

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

				DrawLine(x, drawStart, x, drawEnd);
			}
		}
	}
	return true;
}

int BinarySpacePartitioning::BinSearchForX(float angle)
{
	return BinSearchForX(angle, 0, angleToX.size() - 1);
}

int BinarySpacePartitioning::BinSearchForX(float angle, int left, int right)
{
	int mid = (left + right) / 2;

	// make sure indices are valid
	assert(left < mid);
	assert(mid < right);
	assert(0 <= left);
	assert(right < angleToX.size());

	// NOTE: angleToX IS REVERSE SORTED!
	// THIS MEANS...
	// LEFT IS LARGER, RIGHT IS SMALLER

	// case: angle is on left (larger)
	if (angleToX[left] >= angle && angle > angleToX[mid])
	{
		// case: imperfect match
		if (angle != angleToX[left] && left == mid - 1)
		{
			return left;
		}
		// case: still searching
		return BinSearchForX(angle, left, mid - 1);
	}

	// case: angle is on right (smaller)
	else if (angleToX[mid] >= angle && angle >= angleToX[right])
	{
		// case: imperfect match

		// case: still searching
		return BinSearchForX(angle, mid, right);
	}

	// base case
	else
	{
		return SCREEN_WIDTH;
	}

}
