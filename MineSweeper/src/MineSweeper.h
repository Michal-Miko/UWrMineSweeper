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
	paused
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
	sf::Clock clock;

	// Random engine
	std::random_device dev;
	std::mt19937 rng;

	// Tiles
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
	void clickOnTile(Vector2u pos, sf::Mouse::Button button);
	const Vector2u& getSize() const;
	void setSize(Vector2u size);
	const Tilemap& getFG() const;
	const Tilemap& getBG() const;
	const GState& getGState() const;
	void setGState(GState state);
	GDifficulty getDiff() const;
	void setDiff(GDifficulty d);
	unsigned getMineCount();
	unsigned getFlagCount();
	sf::Time getTimeElapsed();
	void checkVictory();
	void revealMines();
	void reset();

	~MineSweeper();
};