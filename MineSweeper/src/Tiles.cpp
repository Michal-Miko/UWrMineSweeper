#include "Tiles.h"

const Vector2u Tile::fgPos = Vector2u(1, 0);
const Vector2u Tile::bgPos = Vector2u(0, 1);
const Vector2u Tile::flagPos = Vector2u(3, 0);

Tile::Tile(Vector2u tilesetPos) : tilesetPos(tilesetPos) {}

Tile::Tile(ushort x, ushort y) : tilesetPos(x, y) {}

TType Tile::getType() const {
	return type;
}

TState Tile::getState() const {
	return state;
}

const Vector2u & Tile::getTilesetPos() const {
	return tilesetPos;
}

const Vector2u & Tile::getBoardPos() const {
	return boardPos;
}

void Tile::setBoardPos(Vector2u bPos) {
	boardPos = bPos;
}

const vector<Tile*>& Tile::getNeighbours() const {
	return neighbours;
}

void Tile::setNeighbours(vector<Tile*> n) {
	neighbours = n;
}

ushort Tile::getNearbyMines() const {
	return nearbyMines;
}

void Tile::setNearbyMines(ushort count) {
	nearbyMines = count;
	tilesetPos.x = count;
}

void Tile::flag() {
	if (state == TState::hidden)
		state = TState::flagged;
	else if (state == TState::flagged)
		state = TState::hidden;
}

EmptyTile::EmptyTile() : Tile(0, 1) {
	state = TState::hidden;
	type = TType::empty;
}

void EmptyTile::clickedOn() {
	if (state == TState::hidden)
		state = TState::revealed;
	else if (state == TState::revealed)
		state = TState::revealNearby;
}

Mine::Mine() : Tile(5, 0) {
	state = TState::hidden;
	type = TType::mine;
}

void Mine::clickedOn() {
	if (state == TState::hidden)
		state = TState::revealed;
}
