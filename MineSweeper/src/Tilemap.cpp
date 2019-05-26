#include "Tilemap.h"

void Tilemap::addTileVertices(Vector2f texPos, Vector2f pos) {
	vertices.append(Vertex((Vector2f(0, 0) + pos) * renderDim,
		textureDim * texPos
	));

	vertices.append(Vertex((Vector2f(1, 0) + pos) * renderDim,
		textureDim * texPos + Vector2f(textureDim, 0)
	));

	vertices.append(Vertex((Vector2f(1, 1) + pos) * renderDim,
		textureDim * texPos + Vector2f(textureDim, textureDim)
	));

	vertices.append(Vertex((Vector2f(0, 1) + pos) * renderDim,
		textureDim * texPos + Vector2f(0, textureDim)
	));
}

Tilemap::Tilemap(
	Texture tileset, Vector2u size,
	float textureDim, float renderDim,
	Vector2f texPos
) {

	this->tileset = tileset;
	this->size = size;
	this->textureDim = textureDim;
	this->renderDim = renderDim;

	vertices = VertexArray(
		sf::PrimitiveType::Quads,
		(size_t)(size.x*size.y * 4)
	);

	for (unsigned y = 0; y < size.y; y++) {
		for (unsigned x = 0; x < size.x; x++) {
			addTileVertices(
				texPos,
				Vector2f((float)x, (float)y)
			);
		}
	}
}

void Tilemap::draw(RenderTarget & target, RenderStates states) const {
	states.texture = &tileset;
	target.draw(vertices, states);
}
