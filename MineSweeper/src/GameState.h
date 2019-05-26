#include "Tiles.h"
#include <vector>
#include <SFML/Window.hpp>

using std::vector;
using sf::Vector2u;
using sf::Event;


enum class Difficulty {
	easy,
	medium,
	hard,
	very_hard
};

class GameState {
protected:
	Vector2u boardSize;
	vector<vector<Tile*>>* tiles;
public:
	GameState(unsigned w, unsigned h, Difficulty d);
	void handleEvents(Event& e);



};