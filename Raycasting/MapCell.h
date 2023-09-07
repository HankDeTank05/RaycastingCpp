#ifndef MAP_CELL_H
#define MAP_CELL_H

class MapCell
{
public:
	MapCell() = delete;
	MapCell(const MapCell& mc) = delete;
	MapCell& operator=(const MapCell& mc) = delete;
	virtual ~MapCell();

	enum class Type
	{
		Wall,
		Open
	};

	MapCell(Type cellType);

	Type GetCellType() const;

private:
	Type cellType;
};

#endif