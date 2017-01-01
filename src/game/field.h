#pragma once
#include <array>
#include "nodecomponent.h"
#include "block.h"

static const size_t BLOCK_SIZE = 32;

class Field : public NodeComponent {
public:
	Field();
	~Field();


	Node *setBlock(BlockType, size_t, size_t);
	void removeBlock(size_t, size_t);

	virtual void init();
	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};
	
	std::array<std::array<Block *, BLOCK_SIZE>, BLOCK_SIZE> blocks;
private:
};