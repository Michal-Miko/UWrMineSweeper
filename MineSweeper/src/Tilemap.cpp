#include "Tilemap.h"

Tilemap::Tilemap() {}

Tilemap::Tilemap(
	Texture tileset, Vector2u size,
	float textureDim, float renderDim,
	Vector2f texPos
) {
	this->tileset = tileset;
	this->size = size;
	this->textureDim = textureDim;
	this->renderDim = renderDim;

	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(size.x*size.y * 4);

	for (unsigned y = 0; y < size.y; y++)
		for (unsigned x = 0; x < size.x; x++)
			changeTile(Vector2u(x, y), texPos);
}

void Tilemap::changeTile(Vector2u tilePos, Vector2f newTexPos) {
	Vertex* quad = &vertices[(tilePos.y*size.x + tilePos.x) * 4];

	quad[0].position = Vector2f(tilePos.x, tilePos.y) * renderDim;
	quad[1].position = Vector2f(tilePos.x + 1, tilePos.y) * renderDim;
	quad[2].position = Vector2f(tilePos.x + 1, tilePos.y + 1) * renderDim;
	quad[3].position = Vector2f(tilePos.x, tilePos.y + 1) * renderDim;

	quad[0].texCoords = textureDim * newTexPos;
	quad[1].texCoords = textureDim * (newTexPos + Vector2f(1, 0));
	quad[2].texCoords = textureDim * (newTexPos + Vector2f(1, 1));
	quad[3].texCoords = textureDim * (newTexPos + Vector2f(0, 1));
}

void Tilemap::draw(RenderTarget & target, RenderStates states) const {
	states.texture = &tileset;
	target.draw(vertices, states);
}
