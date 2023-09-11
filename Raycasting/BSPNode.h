#ifndef BSP_NODE_H
#define BSP_NODE_H

// forward declarations
class LineDef;
class Vector3;

class BSPNode
{
	BSPNode() = delete;
	BSPNode(const BSPNode& bspn) = delete;
	BSPNode& operator=(const BSPNode& bspn) = delete;
	virtual ~BSPNode();

	BSPNode(LineDef& lineDef);

	bool IsInFront(const Vector3& pos);

private:
	BSPNode* front;
	BSPNode* back;
	LineDef& lineDef;
};

#endif