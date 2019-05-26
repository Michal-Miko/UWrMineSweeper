#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

typedef unsigned short ushort;

using sf::Vector2u;
using std::vector;

enum class TileState {
	revealed,
	hidden,
	flagged
};

enum class TileType {
	empty,
	mine
};

class Tile {
protected:
	Vector2u tilesetPos;
	TileState state;
	TileType type;
public:
	const static Vector2u fgPos;
	const static Vector2u bgPos;

	Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);
	Vector2u getPos() const;
	TileType getType() const;
	virtual void clickedOn() = 0;
};

class EmptyTile : public Tile {
private:
	ushort nearbyMines;
public:
	EmptyTile();
	virtual void clickedOn();
	//void findMines(Vector2u pos, vector<vector<Tile>>& board);
};

class Mine : public Tile {
public:
	Mine();
	virtual void clickedOn();
};


// Powerup tiles?