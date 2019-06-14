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
	vector<Tile*> neighbours;
public:
	// Static
	static TTheme theme;
	static Vector2u texPos;
	static Vector2u flagTexPos;
	static void setTheme(TTheme theme);

	// Local
	TState state;
	Vector2u boardPos;
	TType type{};

	explicit Tile(Vector2u tilesetPos);
	Tile(ushort x, ushort y);
	virtual ~Tile();

	// Neighbours
	void addNeighbours(const vector<vector<Tile*>>& tiles, Vector2u size);
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
