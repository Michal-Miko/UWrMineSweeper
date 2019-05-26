#include "Tiles.h"

Tile::Tile(ushort id) : id(id) {}

EmptyTile::EmptyTile() : Tile(0) {
	state = TileState::hidden;
}

void EmptyTile::clickedOn() {
	if (state == TileState::hidden)
		state = TileState::revealed;
	else {
		// reveal nearby tiles if nearby marked tiles == nearbyMines
	}

}