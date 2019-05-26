#include "Tiles.h"

const Vector2u Tile::fgPos = Vector2u(0, 0);
const Vector2u Tile::bgPos = Vector2u(2, 0);

Tile::Tile(Vector2u tilesetPos) : tilesetPos(tilesetPos) {}

Tile::Tile(ushort x, ushort y) : tilesetPos(x, y) {}

Vector2u Tile::getPos() const {
	return tilesetPos;
}

TileType Tile::getType() const {
	return type;
}

EmptyTile::EmptyTile() : Tile(0, 0) {
	state = TileState::hidden;
	type = TileType::empty;
}

void EmptyTile::clickedOn() {
	if (state == TileState::hidden)
		state = TileState::revealed;
	else {
		// reveal nearby tiles if nearby marked tiles == nearbyMines
	}
}

Mine::Mine() : Tile(4, 0) {
	state = TileState::hidden;
	type = TileType::mine;
}

void Mine::clickedOn() {
	if (state == TileState::hidden)
		state = TileState::revealed;
}
