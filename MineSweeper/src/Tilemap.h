#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
//#include "Tiles.h"

using sf::Drawable;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Texture;
using sf::VertexArray;
using sf::Vertex;
using sf::Vector2u;
using sf::Vector2f;
using std::vector;
using std::floor;

class Tilemap final : public Drawable {
private:
	Texture tileset;
	VertexArray vertices;
	Vector2u size;
	float tDim{};
	float rDim{};
	float spacer{};
public:
	Tilemap();
	Tilemap(
		const Texture& tileset, Vector2u size,
		float tDim, float rDim,
		Vector2f texPos
	);

	void changeTile(Vector2u tilePos, Vector2f newTexPos);
	void draw(RenderTarget& target, RenderStates states) const override;
};
