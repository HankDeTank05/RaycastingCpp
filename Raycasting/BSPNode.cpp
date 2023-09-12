#include "BSPNode.h"

#include "Vector4.h"
#include <cassert>

BSPNode::~BSPNode()
{
	// do nothing
}

BSPNode::BSPNode(LineDef& _lineDef)
	: pBack(nullptr),
	pFront(nullptr),
	lineDef(_lineDef)
{
}

bool BSPNode::IsPointInFront(const Vector4& pos)
{
	assert(pos.GetZ() == 1.0f);

	// right side of wall is front, left side of wall is back

	return false;
}

BSPNode* BSPNode::GetBack() const
{
	return pBack;
}

BSPNode* BSPNode::GetFront() const
{
	return pFront;
}
