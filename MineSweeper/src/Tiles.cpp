#include "Tiles.h"

TTheme Tile::theme = TTheme::black;
Vector2u Tile::fgPos = Vector2u(2 * (int)Tile::theme, 2);
Vector2u Tile::flagPos = Vector2u(2 * (int)Tile::theme + 1, 2);
const Vector2u Tile::bgPos = Vector2u(0, 1);

void Tile::setTheme(TTheme theme) {
	Tile::theme = theme;
	fgPos.x = 2 * (int)theme;
	flagPos.x = 2 * (int)theme + 1;
}

Tile::Tile(Vector2u tilesetPos) : tilesetPos(tilesetPos) {}

Tile::Tile(ushort x, ushort y) : tilesetPos(x, y) {}

TType Tile::getType() const {
	return type;
}

TState Tile::getState() const {
	return state;
}

void Tile::setState(TState s) {
	state = s;
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

Mine::Mine() : Tile(1, 0) {
	state = TState::hidden;
	type = TType::mine;
}

void Mine::clickedOn() {
	if (state == TState::hidden)
		state = TState::revealed;
}
