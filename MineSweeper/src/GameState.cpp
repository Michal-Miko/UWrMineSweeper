#include "GameState.h"

GameState::GameState(unsigned w, unsigned h, Difficulty d) :
	boardSize(w, h) {
	tiles = new vector<vector<Tile*>>(h, vector<Tile*>(w, new EmptyTile()));
}