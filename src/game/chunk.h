#pragma once
#include <array>
#include "block.h"

class Chunk {
public:
	Chunk() {};

	const Block &at(const glm::vec3 &position) const { return blocks.at((int)position.y).blocks.at((int)position.x).at((int)position.z); };
	const Block &at(int x, int y, int z) const { return blocks.at(y).blocks.at(x).at(z); };
	void set(const glm::vec3 &position, Block &block) { blocks.at((int)position.y).blocks.at((int)position.x).at((int)position.z) = block; };
	void set(int x, int y, int z, Block &block) { blocks.at(y).blocks.at(x).at(z) = block; };

	// VertexBuffer *generateVertexBuffer() { return nullptr; };
	// static const int32_t CHUNK_SIZE;
private:
	struct Layer {
		std::array<std::array<Block, 32>, 32> blocks;
	};
	std::array<Layer, 32> blocks;
};