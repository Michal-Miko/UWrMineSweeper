#include "Tiles.h"

TTheme Tile::theme = TTheme::black;
Vector2u Tile::fgPos = Vector2u(2 * int(theme), 2);
Vector2u Tile::flagPos = Vector2u(2 * int(theme) + 1, 2);
const Vector2u Tile::bgPos = Vector2u(0, 1);

void Tile::setTheme(TTheme theme) {
	Tile::theme = theme;
	fgPos.x = 2 * (int(theme) % 4);
	fgPos.y = ceil(int(theme) / 4) + 2;
	flagPos.x = fgPos.x + 1;
	flagPos.y = ceil(int(theme) / 4) + 2;
}

Tile::Tile(const Vector2u tilesetPos) : tilesetPos(tilesetPos), type(), state() {}

Tile::Tile(const ushort x, const ushort y) : tilesetPos(x, y), type(), state() {}

Tile::~Tile() = default;

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
			Vector2f(flagPos)
		);
		(*fc)++;
		state = TState::flagged;
	}
	else if (state == TState::flagged) {
		tm->changeTile(
			Vector2u(boardPos.x, boardPos.y),
			Vector2f(fgPos)
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
