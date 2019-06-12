#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Tilemap.h"

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
	any
};

enum class TType {
	empty,
	mine,
	flare,
	any
};

class Tile {
protected:
	Vector2u tilesetPos;
public:
	vector<Tile*> neighbours;
	Vector2u boardPos;
	TType type;
	TState state;
	static TTheme theme;
	static Vector2u fgPos;
	static Vector2u flagPos;
	const static Vector2u bgPos;
	static void setTheme(TTheme theme);

	Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);

	// Neighbours
	ushort countNearby(TState state, TType type);

	void flag(Tilemap* tm, unsigned* fc);
	virtual bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) = 0;
};

class EmptyTile : public Tile {
protected:
	ushort nearbyMines;
	void setNearbyMines();
public:
	EmptyTile();
	virtual bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};

class Mine : public Tile {
public:
	Mine();
	virtual bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};

class Flare : public EmptyTile {
public:
	Flare();
	virtual bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};