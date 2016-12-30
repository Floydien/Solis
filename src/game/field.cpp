#include "field.h"
#include "rendercomponent.h"

Field::Field() {
	for (size_t y = 0; y < BLOCK_SIZE; y++) {
		for (size_t x = 0; x < BLOCK_SIZE; x++) {
			blocks[x][y] = new Block(BlockType::eEmpty);
		}
	}
}
Field::~Field() {
	for (size_t y = 0; y < BLOCK_SIZE; y++) {
		for (size_t x = 0; x < BLOCK_SIZE; x++) {
			if (blocks[x][y]->getType() == BlockType::eEmpty) {
				delete blocks[x][y];
			}
		}
	}
}


Node *Field::setBlock(BlockType type, size_t x, size_t y) {
	Node *out = nullptr;

	if(x >= BLOCK_SIZE || y >= BLOCK_SIZE) {
		return out;
	}

	if (blocks[x][y]->getType() != BlockType::eEmpty) {
		/* remove the old node */
	}

	if (type != BlockType::eEmpty) {
		blocks[x][y]->setContent(type);
		out = new Node(Transform(glm::vec3(x,0,y)), "block " + std::to_string(x) + " " + std::to_string(y));
		out->addComponent(
			new RenderComponent(
				parent->getScene()->getMesh("tree.obj"),
				new Material(
					parent->getScene()->getVideoDriver()->getTexture("solis.png"))));
		out->addComponent(
			blocks[x][y]);

		parent->addChild(out);
	}

	return out;	
}

void Field::removeBlock(size_t x, size_t y) {
	/* return if out of scope or if the block is already empty */
	if (x >= BLOCK_SIZE || y >= BLOCK_SIZE ||
		blocks[x][y]->getType() == BlockType::eEmpty) {
		return;
	}


}