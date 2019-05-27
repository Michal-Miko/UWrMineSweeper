#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

typedef unsigned short ushort;

using sf::Vector2u;
using std::vector;

enum class TState {
	revealed,
	hidden,
	flagged,
	revealNearby,
	any
};

enum class TType {
	empty,
	mine,
	any
};

class Tile {
protected:
	vector<Tile*> neighbours;
	Vector2u tilesetPos;
	Vector2u boardPos;
	TState state;
	TType type;
	ushort nearbyMines;
public:
	const static Vector2u fgPos;
	const static Vector2u bgPos;
	const static Vector2u flagPos;

	Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);
	TType getType() const;
	TState getState() const;
	const Vector2u& getTilesetPos() const;
	const Vector2u& getBoardPos() const;
	void setBoardPos(Vector2u bPos);

	// Neighbours
	const vector<Tile*>& getNeighbours() const;
	void  setNeighbours(vector<Tile*> n);
	// Nearby Mines
	ushort getNearbyMines() const;
	void setNearbyMines(ushort count);

	void flag();
	virtual void clickedOn() = 0;
};

class EmptyTile : public Tile {
private:
public:
	EmptyTile();
	virtual void clickedOn();
};

class Mine : public Tile {
public:
	Mine();
	virtual void clickedOn();
};


// Powerup tiles?