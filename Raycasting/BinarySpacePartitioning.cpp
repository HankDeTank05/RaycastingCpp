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

	// set player pos and dir
	pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	dir = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	plane = Vector4(0.66f, 0.0f, 0.0f, 0.0f);

	// create sector map
	map.AddLineDef(LineDef(-5.0f, -10.0f, 5.0f, -10.0f));

	return true;
}

bool BinarySpacePartitioning::OnUserUpdate(float fElapsedTime)
{
	std::vector<LineDef> lineDefs = map.GetLineDefs();

	for (int ld = 0; ld < lineDefs.size(); ld++)
	{
		LineDef currLineDef = lineDefs[ld];
		if (dir.dot(currLineDef.GetNormal()) < 0.0f)
		{
			// draw the linedef from the leftmost column to the rightmost column

			int leftX = 0;
			int rightX = SCREEN_WIDTH;

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
