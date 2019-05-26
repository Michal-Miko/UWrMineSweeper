#include "Tiles.h"
#include <vector>
#include <SFML/Window.hpp>

using std::vector;

class GameState {
protected:
	vector<vector<Tile>> tiles;
public:
	void handleEvents(sf::Event& e);
};