#include "Tilemap.h"

Tilemap::Tilemap() {}

Tilemap::Tilemap(
	Texture tileset, Vector2u size,
	float tDim, float rDim,
	Vector2f texPos
) {
	this->tileset = tileset;
	this->size = size;
	this->tDim = tDim;
	this->rDim = rDim;

	spacer = 1;

	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(size.x*size.y * 4);

	for (unsigned y = 0; y < size.y; y++)
		for (unsigned x = 0; x < size.x; x++)
			changeTile(Vector2u(x, y), texPos);
}

void Tilemap::changeTile(Vector2u tilePos, Vector2f newTexPos) {
	Vertex* quad = &vertices[(tilePos.y*size.x + tilePos.x) * 4];

	float px = floor((float)tilePos.x);
	float py = floor((float)tilePos.y);
	quad[0].position = Vector2f(px * rDim, py * rDim);
	quad[1].position = Vector2f((px + 1) * rDim, py * rDim);
	quad[2].position = Vector2f((px + 1) * rDim, (py + 1) * rDim);
	quad[3].position = Vector2f(px * rDim, (py + 1) * rDim);

	float tx = floor(newTexPos.x);
	float ty = floor(newTexPos.y);
	float txSpacer = tx * spacer;
	float tySpacer = ty * spacer;

	quad[0].texCoords = (tDim * newTexPos) + Vector2f(spacer, spacer);
	quad[1].texCoords = tDim * (newTexPos + Vector2f(1, 0));
	quad[2].texCoords = tDim * (newTexPos + Vector2f(1, 1));
	quad[3].texCoords = tDim * (newTexPos + Vector2f(0, 1));

	quad[0].texCoords = Vector2f(tx * tDim + txSpacer, ty * tDim + tySpacer);
	quad[1].texCoords = Vector2f((tx + 1) * tDim + txSpacer, ty * tDim + tySpacer);
	quad[2].texCoords = Vector2f((tx + 1) * tDim + txSpacer, (ty + 1) * tDim + tySpacer);
	quad[3].texCoords = Vector2f(tx * tDim + txSpacer, (ty + 1) * tDim + tySpacer);
}

void Tilemap::draw(RenderTarget & target, RenderStates states) const {
	states.texture = &tileset;
	target.draw(vertices, states);
}
