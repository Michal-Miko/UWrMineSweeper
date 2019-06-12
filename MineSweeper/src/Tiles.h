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

class Tile {  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
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

	explicit Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);
	virtual ~Tile();

	// Neighbours
	ushort countNearby(TState state, TType type);

	void flag(Tilemap* tm, unsigned* fc);
	virtual bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc);
};

class EmptyTile : public Tile {
protected:
	ushort nearbyMines{};
	void setNearbyMines();
public:
	EmptyTile();
	bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};

class Mine final : public Tile {
public:
	Mine();
	bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};

class Flare final : public EmptyTile {
public:
	Flare();
	bool clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) override;
};
