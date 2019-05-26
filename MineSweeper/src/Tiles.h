#include <SFML/System/Vector2.hpp>
#include <vector>

typedef unsigned short ushort;

using sf::Vector2u;
using std::vector;

enum class TileState {
	hidden,
	revealed
};


class Tile {
protected:
	const ushort id;
	TileState state;
public:
	Tile(ushort id);
	virtual void clickedOn() = 0;
};

class EmptyTile : public Tile {
private:
	ushort nearbyMines;
public:
	EmptyTile();
	virtual void clickedOn();
	void findMines(Vector2u pos, vector<vector<Tile>>& board);
};

class Mine : public Tile {
public:
	Mine();
};


// Powerup tiles?