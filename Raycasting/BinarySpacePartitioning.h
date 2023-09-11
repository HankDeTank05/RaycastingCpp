#ifndef BINARY_SPACE_PARTITIONING_H
#define BINARY_SPACE_PARTITIONING_H

class BinarySpacePartitioning
{
public:
	BinarySpacePartitioning();
	BinarySpacePartitioning(const BinarySpacePartitioning& bsp) = delete;
	BinarySpacePartitioning& operator=(const BinarySpacePartitioning& bsp) = delete;
	virtual ~BinarySpacePartitioning();
};

#endif