#include "field.h"

Node *Field::setBlock(BlockType type, size_t x, size_t y) {
	if (blocks[x][y].getType() != BlockType::eEmpty) {
		/* remove the old node */
	}
	if (type != BlockType::eEmpty) {
		blocks[x][y] = Block(type);
		
	}
	/* TODO: return real node */
	return nullptr;	
}