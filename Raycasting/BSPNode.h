#ifndef BSP_NODE_H
#define BSP_NODE_H

// forward declarations
class LineDef;
class Vector4;

class BSPNode
{
public:
	BSPNode() = delete;
	BSPNode(const BSPNode& bspn) = delete;
	BSPNode& operator=(const BSPNode& bspn) = delete;
	virtual ~BSPNode();

	BSPNode(LineDef& lineDef);

	bool IsPointInFront(const Vector4& pos);

	BSPNode* GetBack() const;
	BSPNode* GetFront() const;

private:
	BSPNode* pBack;
	BSPNode* pFront;
	LineDef& lineDef;
};

#endif