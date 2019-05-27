#pragma once

#include "Tilemap.h"
#include "Tiles.h"
#include "Utils.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

using std::vector;
using sf::Vector2u;
using sf::Event;


enum class GDifficulty {
	easy,
	medium,
	hard,
	veryHard,
	custom
};

enum class GState {
	running,
	loss,
	victory,
	startup
};

class MineSweeper {
protected:
	// Logic
	vector<vector<Tile*>> tiles;
	unsigned mineCount;
	unsigned flagCount;
	unsigned hiddenCount;
	Vector2u size;
	GState gState;
	GDifficulty diff;
	std::string assets;

	// Random engine
	std::default_random_engine generator;

	void updateTile(Tile* t);
	ushort countNearby(const Tile* t, TState state, TType type);
	void addNeighbours(Tile* t);

	// Graphics
	sf::Texture tileset;
	Tilemap fgTilemap;
	Tilemap bgTilemap;
public:
	MineSweeper();
	MineSweeper(std::string assets, GDifficulty diff);
	MineSweeper(std::string assets, Vector2u customSize);
	void handleEvents(const Event & event, const sf::Window & window);
	const Vector2u& getSize() const;
	const Tilemap& getFG() const;
	const Tilemap& getBG() const;
	const GState& getGState() const;
	void reset();

	~MineSweeper();
};