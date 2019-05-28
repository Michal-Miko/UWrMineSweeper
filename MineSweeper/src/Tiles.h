#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

typedef unsigned short ushort;

using sf::Vector2u;
using std::vector;

enum class TTheme {
	blue,
	green,
	black,
	white,
	yellow,
	brown,
	purple,
	gray
};

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
	static TTheme theme;
	static Vector2u fgPos;
	static Vector2u flagPos;
	const static Vector2u bgPos;
	static void setTheme(TTheme theme);

	Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);
	TType getType() const;
	TState getState() const;
	void setState(TState s);
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

// TODO: Flare
// Powerup tiles?