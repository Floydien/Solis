#pragma once
#include <array>
#include "nodecomponent.h"
#include "block.h"

class Field : public NodeComponent {
public:
	Field() {};

	Node *setBlock(BlockType type, size_t x, size_t y);

	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};
	
	std::array<std::array<Block, 32>, 32> blocks;
private:
};