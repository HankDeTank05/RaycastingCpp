#include "BSPNode.h"

#include "Vector3.h"
#include <cassert>

bool BSPNode::IsInFront(const Vector3& pos)
{
	assert(pos.GetZ() == 1.0f);
	return false;
}
