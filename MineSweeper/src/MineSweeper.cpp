#include "MineSweeper.h"

void MineSweeper::updateTile(Tile* t) {
	unsigned x = t->getBoardPos().x;
	unsigned y = t->getBoardPos().y;
	Tile* tile = tiles[y][x];

	switch (tile->getState()) {
	case TState::revealed:
		fgTilemap.changeTile(
			Vector2u(x, y),
			(Vector2f)tiles[y][x]->getTilesetPos()
		);

		if (tile->getType() == TType::mine)
			gState = GState::loss;

		if (tile->getNearbyMines() == 0) {
			for (auto t : tile->getNeighbours())
				if (t->getState() == TState::hidden) {
					t->clickedOn();
					updateTile(t);
					hiddenCount--;
				}
		}
		break;

	case TState::flagged:
		fgTilemap.changeTile(
			Vector2u(x, y),
			(Vector2f)Tile::flagPos
		);
		flagCount++;
		break;

	case TState::hidden:
		fgTilemap.changeTile(
			Vector2u(x, y),
			(Vector2f)Tile::fgPos
		);
		flagCount--;
		break;

	case TState::revealNearby:
		if (countNearby(tile, TState::flagged, TType::any) == tile->getNearbyMines()) {
			for (auto t : tile->getNeighbours())
				if (t->getState() == TState::hidden) {
					t->clickedOn();
					updateTile(t);
					hiddenCount--;
				}
		}
	default:
		break;
	}
}

ushort MineSweeper::countNearby(const Tile* t, TState state, TType type) {
	ushort count = 0;

	for (auto t : t->getNeighbours()) {
		if (state == TState::any || state == t->getState())
			if (type == TType::any || type == t->getType())
				count++;
	}

	return count;
}

void MineSweeper::addNeighbours(Tile* t) {
	unsigned x = t->getBoardPos().x;
	unsigned y = t->getBoardPos().y;
	vector<Tile*> neighbours;

	// left
	if (x > 0)
		neighbours.push_back(tiles[y][x - 1]);
	// top left
	if (x > 0 && y > 0)
		neighbours.push_back(tiles[y - 1][x - 1]);
	// top
	if (y > 0)
		neighbours.push_back(tiles[y - 1][x]);
	// top right
	if (x < size.x - 1 && y > 0)
		neighbours.push_back(tiles[y - 1][x + 1]);
	// right
	if (x < size.x - 1)
		neighbours.push_back(tiles[y][x + 1]);
	// bottom right
	if (x < size.x - 1 && y < size.y - 1)
		neighbours.push_back(tiles[y + 1][x + 1]);
	// bottom
	if (y < size.y - 1)
		neighbours.push_back(tiles[y + 1][x]);
	// bottom left
	if (x > 0 && y < size.y - 1)
		neighbours.push_back(tiles[y + 1][x - 1]);

	t->setNeighbours(neighbours);
}

MineSweeper::MineSweeper() {
	rng = std::mt19937(dev());
}

MineSweeper::MineSweeper(std::string assets, GDifficulty diff = GDifficulty::medium) : MineSweeper() {
	this->diff = diff;
	this->assets = assets;

	gState = GState::startup;
	reset();
}

MineSweeper::MineSweeper(std::string assets, Vector2u customSize) : MineSweeper() {
	this->diff = GDifficulty::custom;
	this->assets = assets;
	this->size = customSize;

	gState = GState::startup;
	reset();

	size = customSize;
}

void MineSweeper::clickOnTile(Vector2u pos, sf::Mouse::Button button) {
	if (button == sf::Mouse::Left) {
		if (tiles[pos.y][pos.x]->getState() == TState::hidden)
			hiddenCount--;
		tiles[pos.y][pos.x]->clickedOn();
		updateTile(tiles[pos.y][pos.x]);
	}

	if (button == sf::Mouse::Right) {
		tiles[pos.y][pos.x]->flag();
		updateTile(tiles[pos.y][pos.x]);
	}
}

const Vector2u & MineSweeper::getSize() const {
	return size;
}

void MineSweeper::setSize(Vector2u size) {
	this->size = size;
}

const Tilemap & MineSweeper::getFG() const {
	return fgTilemap;
}

const Tilemap & MineSweeper::getBG() const {
	return bgTilemap;
}

const GState & MineSweeper::getGState() const {
	return gState;
}

void MineSweeper::setGState(GState state) {
	gState = state;
}

GDifficulty MineSweeper::getDiff() const {
	return diff;
}

void MineSweeper::setDiff(GDifficulty d) {
	diff = d;
}

unsigned MineSweeper::getMineCount() {
	return mineCount;
}

unsigned MineSweeper::getFlagCount() {
	return flagCount;
}

sf::Time MineSweeper::getTimeElapsed() {
	return clock.getElapsedTime();
}

void MineSweeper::checkVictory() {
	if (hiddenCount == mineCount)
		gState = GState::victory;
}

void MineSweeper::revealMines() {
	for (auto v : tiles)
		for (auto t : v)
			if (t->getType() == TType::mine) {
				if (t->getState() == TState::hidden) {
					t->clickedOn();
					updateTile(t);
					hiddenCount--;
				}
				else if (t->getState() == TState::flagged) {
					t->flag();
					t->clickedOn();
					updateTile(t);
					hiddenCount--;
				}
			}
}

void MineSweeper::reset() {
	if (gState != GState::startup)
		for (auto v : tiles)
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

	mineCount = size.x * size.y * (18 + (int)diff) / 100;

	gState = GState::running;
	hiddenCount = size.x*size.y;

	// Vector of empty tiles
	tiles = vector<vector<Tile*>>(size.y, vector<Tile*>());
	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++)
			tiles[y].push_back(new EmptyTile());
	}

	// Mine generation
	std::uniform_int_distribution<unsigned> distHor(0, size.x - 1);
	std::uniform_int_distribution<unsigned> distVer(0, size.y - 1);
	unsigned x, y;
	for (unsigned i = 0; i < mineCount; i++) {
		x = distHor(rng);
		y = distVer(rng);
		while (tiles[y][x]->getType() == TType::mine) {
			x = distHor(rng);
			y = distVer(rng);
		}
		tiles[y][x] = new Mine();
	}

	// Adding neighbours, setting positions
	for (unsigned y = 0; y < size.y; y++)
		for (unsigned x = 0; x < size.x; x++) {
			tiles[y][x]->setBoardPos(Vector2u(x, y));
			addNeighbours(tiles[y][x]);
		}

	// Set mine counts
	for (auto v : tiles)
		for (auto t : v)
			if (t->getType() == TType::empty)
				t->setNearbyMines(countNearby(t, TState::any, TType::mine));

	flagCount = 0;

	// Graphics
	tileset.loadFromFile(assets + "tilemap.png");
	fgTilemap = Tilemap(tileset, size, 16, 16, (Vector2f)Tile::fgPos);
	bgTilemap = Tilemap(tileset, size, 16, 16, (Vector2f)Tile::bgPos);

	// Clock
	clock.restart();
}

MineSweeper::~MineSweeper() {
	for (auto v : tiles)
		for (auto t : v)
			delete t;
}