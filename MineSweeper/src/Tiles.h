#include <SFML/System/Vector2.hpp>
#include <vector>

typedef unsigned short ushort;

using sf::Vector2u;
using std::vector;

class Tile {
protected:
	ushort id;
	short state;
public:
	Tile();
	virtual void clickedOn() = 0;
};

class EmptyTile : public Tile {
private:
	ushort nearbyMines;
public:
	EmptyTile();
	void findMines(Vector2u pos, vector<vector<Tile>>& board);
};

class Mine : public Tile {
public:
	Mine();
};


// Powerup tiles?