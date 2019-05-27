#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Tiles.h"

using sf::Drawable;
using sf::RenderTarget;
using sf::RenderStates;
using sf::Texture;
using sf::VertexArray;
using sf::Vertex;
using sf::Vector2u;
using sf::Vector2f;
using std::vector;

class Tilemap : public Drawable {
private:
	Texture tileset;
	VertexArray vertices;
	Vector2u size;
	float textureDim;
	float renderDim;
public:
	Tilemap();
	Tilemap(
		Texture tileset, Vector2u size,
		float textureDim, float renderDim,
		Vector2f texPos
	);

	void changeTile(Vector2u tilePos, Vector2f newTexPos);
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};