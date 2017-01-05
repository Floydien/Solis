#pragma once
#include <array>
#include "nodecomponent.h"
#include "block.h"

static const size_t FIELD_SIZE = 32;

class Field : public NodeComponent {
public:

	Node *setBlock(BlockType, size_t, size_t);
	void removeBlock(size_t, size_t);

	bool build(BuildingType, size_t, size_t);

	virtual void init();
	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};
	
	std::array<std::array<Block *, FIELD_SIZE>, FIELD_SIZE> blocks;
private:
};