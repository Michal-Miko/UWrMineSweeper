#pragma once

#include "Tiles.h"
#include <vector>
#include <SFML/Window.hpp>
#include <random>

using std::vector;
using sf::Vector2u;
using sf::Event;


enum class Difficulty {
	easy,
	medium,
	hard,
	very_hard
};

class GameState {
protected:
	unsigned mineCount;
	Vector2u boardSize;
	vector<vector<Tile*>>* tiles;
public:
	GameState(Difficulty diff);
	void handleEvents(Event& e);
	const vector<vector<Tile*>>* getTilesPtr() const;
	const Vector2u getSize() const;

	~GameState();
};