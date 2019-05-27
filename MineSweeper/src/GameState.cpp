#include "GameState.h"

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
		hiddenCount--;
		if (tile->getType() == TType::mine) {
			gState = GState::loss;
			std::cout << "Loss\n";
		}

		if (hiddenCount == mineCount && gState == GState::running) {
			gState = GState::victory;
			std::cout << "Victory\n";
		}

		if (tile->getNearbyMines() == 0) {
			for (auto t : tile->getNeighbours())
				if (t->getState() == TState::hidden) {
					t->clickedOn();
					updateTile(t);
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

MineSweeper::MineSweeper() {}

MineSweeper::MineSweeper(std::string assets, GDifficulty diff = GDifficulty::medium) {
	this->diff = diff;
	this->assets = assets;


	gState = GState::startup;
	reset();
}

MineSweeper::MineSweeper(std::string assets, Vector2u customSize) {
	this->diff = GDifficulty::custom;
	this->assets = assets;
	this->size = customSize;

	gState = GState::startup;
	reset();

	size = customSize;
}

void MineSweeper::handleEvents(const Event & event, const sf::Window & window) {
	if (event.type == Event::MouseButtonPressed) {
		// Position
		unsigned mx = event.mouseButton.x;
		unsigned my = event.mouseButton.y;

		// Mapping to block pos
		mx = (unsigned)map((float)mx, 0.0f, (float)window.getSize().x, 0.0f, (float)size.x);
		my = (unsigned)map((float)my, 0.0f, (float)window.getSize().y, 0.0f, (float)size.y);

		if (event.mouseButton.button == sf::Mouse::Left) {
			tiles[my][mx]->clickedOn();
			updateTile(tiles[my][mx]);
		}
		if (event.mouseButton.button == sf::Mouse::Right) {
			tiles[my][mx]->flag();
			updateTile(tiles[my][mx]);
		}
	}
}

const Vector2u& MineSweeper::getSize() const {
	return size;
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
		size = Vector2u(12, 21);
		break;
	case GDifficulty::hard:
		size = Vector2u(16, 30);
		break;
	case GDifficulty::veryHard:
		size = Vector2u(20, 36);
	case GDifficulty::custom:
	default:
		break;
	}

	mineCount = size.x * size.y * 16 / 100;

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
		x = distHor(generator);
		y = distVer(generator);
		while (tiles[y][x]->getType() == TType::mine) {
			x = distHor(generator);
			y = distVer(generator);
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

	// Graphics
	tileset.loadFromFile(assets + "tilemap.png");

	fgTilemap = Tilemap(tileset, size, 16, 16, (Vector2f)Tile::fgPos);
	bgTilemap = Tilemap(tileset, size, 16, 16, (Vector2f)Tile::bgPos);
}

MineSweeper::~MineSweeper() {
	for (auto v : tiles)
		for (auto t : v)
			delete t;
}