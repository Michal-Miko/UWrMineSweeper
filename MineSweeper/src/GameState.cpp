#include "GameState.h"

GameState::GameState(Difficulty diff = Difficulty::medium) {
	switch (diff) {
	case Difficulty::easy:
		boardSize = Vector2u(8, 8);
		mineCount = 10;
		break;
	case Difficulty::medium:
		boardSize = Vector2u(12, 21);
		mineCount = 40;
		break;
	case Difficulty::hard:
		boardSize = Vector2u(16, 30);
		mineCount = 100;
		break;
	case Difficulty::very_hard:
		boardSize = Vector2u(20, 36);
		mineCount = 160;
		break;
	}

	tiles = new vector<vector<Tile*>>(boardSize.y, vector<Tile*>());

	for (unsigned y = 0; y < boardSize.y; y++) {
		for (unsigned x = 0; x < boardSize.x; x++)
			(*tiles)[y].push_back(new EmptyTile());
	}

	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned> distHor(0, boardSize.x - 1);
	std::uniform_int_distribution<unsigned> distVer(0, boardSize.y - 1);
	unsigned x, y;
	for (unsigned i = 0; i < mineCount; i++) {
		x = distHor(generator);
		y = distVer(generator);
		while ((*tiles)[y][x]->getType() == TileType::mine) {
			x = distHor(generator);
			y = distVer(generator);
		}
		delete (*tiles)[y][x];
		(*tiles)[y][x] = new Mine();
	}
}

const vector<vector<Tile*>>* GameState::getTilesPtr() const {
	return tiles;
}

const Vector2u GameState::getSize() const {
	return boardSize;
}

GameState::~GameState() {
	for (auto v : *tiles)
		for (auto t : v)
			delete t;

	delete tiles;
}