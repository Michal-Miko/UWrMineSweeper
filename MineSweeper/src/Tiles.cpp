#include "Tiles.h"

TTheme Tile::theme = TTheme::black;
Vector2u Tile::texPos = Vector2u(2 * int(theme), 2);
Vector2u Tile::flagTexPos = Vector2u(2 * int(theme) + 1, 2);

void Tile::setTheme(TTheme theme) {
	Tile::theme = theme;
	texPos.x = 2 * (int(theme) % 4);
	texPos.y = ceil(int(theme) / 4) + 2;
	flagTexPos.x = texPos.x + 1;
	flagTexPos.y = texPos.y;
}

Tile::Tile(const Vector2u tilesetPos) : tilesetPos(tilesetPos), state() {}

Tile::Tile(const ushort x, const ushort y) : tilesetPos(x, y), state() {}

Tile::~Tile() = default;
void Tile::addNeighbours(const vector<vector<Tile*>>& tiles, const Vector2u size) {
	const auto x = boardPos.x;
	const auto y = boardPos.y;

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
}

ushort Tile::countNearby(const TState state, const TType type) {
	ushort count = 0;

	for (auto t : neighbours) {
		if (state == TState::any || state == t->state)
			if (type == TType::any || type == t->type)
				count++;
	}

	return count;
}

void Tile::flag(Tilemap* tm, unsigned* fc) {
	if (state == TState::hidden) {
		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(flagTexPos)
		);
		(*fc)++;
		state = TState::flagged;
	}
	else if (state == TState::flagged) {
		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(texPos)
		);
		(*fc)--;
		state = TState::hidden;
	}
}

bool Tile::clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) { return false; }

EmptyTile::EmptyTile() : Tile(0, 1) {
	state = TState::hidden;
	type = TType::empty;
}

void EmptyTile::setNearbyMines() {
	nearbyMines = this->countNearby(TState::any, TType::mine);
	if (type == TType::empty)
		tilesetPos.x = nearbyMines;
}

bool EmptyTile::clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) {
	if (state == TState::hidden) {
		setNearbyMines();

		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(tilesetPos)
		);

		state = TState::revealed;
		(*hc)--;

		if (nearbyMines == 0)
			for (auto t : neighbours)
				if (t->state == TState::hidden)
					t->clickedOn(tm, hc, fc);
	}
	else if (state == TState::revealed) {
		if (countNearby(TState::flagged, TType::any) == nearbyMines) {
			for (auto t : neighbours)
				if (t->state == TState::hidden)
					if (t->clickedOn(tm, hc, fc))
						return true; // clicked on a mine
		}
	}
	return false;
}

Mine::Mine() : Tile(1, 0) {
	state = TState::hidden;
	type = TType::mine;
}

bool Mine::clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) {
	if (state == TState::hidden) {
		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(tilesetPos)
		);
		state = TState::revealed;
		return true;
	}
	return false;
}

Flare::Flare() {
	tilesetPos = Vector2u(2, 0);
	state = TState::hidden;
	type = TType::flare;
}

// Return true if clicked on a mine, false otherwise
bool Flare::clickedOn(Tilemap* tm, unsigned* hc, unsigned* fc) {
	if (state == TState::hidden) {
		setNearbyMines();

		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(tilesetPos)
		);

		(*hc)--;
		state = TState::revealed;
	}
	else {
		for (auto t : neighbours) {
			if (t->type == TType::mine && t->state == TState::hidden)
				t->flag(tm, fc);
			else if (t->state == TState::hidden)
				t->clickedOn(tm, hc, fc);
		}
	}
	return false;
}
