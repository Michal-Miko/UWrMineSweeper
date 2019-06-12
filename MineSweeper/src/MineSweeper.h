#pragma once

#include "Tilemap.h"
#include "Tiles.h"
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
	startup,
	paused,
	usingFlare
};

class MineSweeper {
protected:
	// Logic
	vector<vector<Tile*>> tiles;
	std::string assets;

	// Random engine
	std::random_device dev;
	std::mt19937 rng;

	// Tiles
	void addNeighbours(Tile* t);

	// Graphics
	sf::Texture tileset;
public:
	sf::Clock clock;
	Vector2u size;
	GDifficulty diff;
	unsigned mineCount;
	unsigned flagCount;
	unsigned hiddenCount;
	unsigned flares;
	unsigned flareCount;

	Tilemap tilemap;
	GState gState;

	MineSweeper();
	MineSweeper(std::string assets, GDifficulty diff);
	MineSweeper(std::string assets, Vector2u customSize);
	void clickOnTile(Vector2u pos, sf::Mouse::Button button);
	void checkVictory();
	void revealMines();
	void reset();

	~MineSweeper();
};