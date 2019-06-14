#include "MineSweeper.h"

MineSweeper::MineSweeper() { rng = std::mt19937(dev()); }   // NOLINT

MineSweeper::MineSweeper(const std::string& assets, const GDifficulty diff = GDifficulty::medium) : MineSweeper() {
	this->diff = diff;
	this->assets = assets;

	gState = GState::startup;
	reset();
}

MineSweeper::MineSweeper(const std::string& assets, const Vector2u customSize) : MineSweeper() {
	this->diff = GDifficulty::custom;
	this->assets = assets;
	this->size = customSize;

	gState = GState::startup;
	reset();

	size = customSize;
}

void MineSweeper::clickOnTile(const Vector2u pos, const sf::Mouse::Button button) {
	if (button == sf::Mouse::Left) {
		if (tiles[pos.y][pos.x]->clickedOn(&tilemap, &hiddenCount, &flagCount))
			gState = GState::loss;
	}

	if (button == sf::Mouse::Right) { tiles[pos.y][pos.x]->flag(&tilemap, &flagCount); }
}

void MineSweeper::checkVictory() {
	if (hiddenCount == mineCount)
		gState = GState::victory;
}

void MineSweeper::revealMines() {
	for (const auto& v : tiles)
		for (auto t : v)
			if (t->type == TType::mine) {
				if (t->state == TState::hidden) { t->clickedOn(&tilemap, &hiddenCount, &flagCount); }
				else if (t->state == TState::flagged) {
					t->flag(&tilemap, &flagCount);
					t->clickedOn(&tilemap, &flagCount, &flagCount);
				}
			}
}

void MineSweeper::reset() {
	if (gState != GState::startup)
		for (const auto& v : tiles)
			for (auto t : v)
				delete t;

	// Set diff
	switch (diff) {
	case GDifficulty::easy:
		size = Vector2u(8, 8);
		break;
	case GDifficulty::medium:
		size = Vector2u(14, 18);
		break;
	case GDifficulty::hard:
		size = Vector2u(20, 24);
		break;
	case GDifficulty::veryHard:
		size = Vector2u(24, 30);
	case GDifficulty::custom:
	default:
		break;
	}

	flares = 0;
	mineCount = size.x * size.y * (16 + int(diff)) / 100;
	flareCount = 1 + size.x * size.y * 3 / 100;

	gState = GState::running;
	hiddenCount = size.x * size.y;

	// Vector of empty tiles
	tiles = vector<vector<Tile*>>(size.y, vector<Tile*>());
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++)
			tiles[y].push_back(new EmptyTile());
	}

	// Flare, Mine generation
	const std::uniform_int_distribution<unsigned> distHor(0, size.x - 1);
	const std::uniform_int_distribution<unsigned> distVer(0, size.y - 1);
	unsigned x, y;

	for (unsigned i = 0; i < flareCount; i++) {
		x = distHor(rng);
		y = distVer(rng);
		while (tiles[y][x]->type == TType::flare) {
			x = distHor(rng);
			y = distVer(rng);
		}
		tiles[y][x] = new Flare();
	}

	for (unsigned i = 0; i < mineCount; i++) {
		x = distHor(rng);
		y = distVer(rng);
		while (tiles[y][x]->type == TType::mine || tiles[y][x]->type == TType::flare) {
			x = distHor(rng);
			y = distVer(rng);
		}
		tiles[y][x] = new Mine();
	}

	// Adding neighbours, setting positions
	for (unsigned i = 0; i < size.y; i++)
		for (unsigned j = 0; j < size.x; j++) {
			tiles[i][j]->boardPos = Vector2u(j, i);
			tiles[i][j]->addNeighbours(tiles, size);
		}

	flagCount = 0;

	// Graphics
	tileset.loadFromFile(assets + "tilemap.png");
	tilemap = Tilemap(tileset, size, 16, 16, Vector2f(Tile::texPos));

	// Clock
	clock.restart();
}

MineSweeper::~MineSweeper() {
	for (const auto& v : tiles)
		for (auto t : v)
			delete t;
}
