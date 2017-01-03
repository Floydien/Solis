#include "field.h"
#include "rendercomponent.h"

Field::Field() {
}

Field::~Field() {

}

void Field::init() {
	for (size_t y = 0; y < BLOCK_SIZE; y++) {
		for (size_t x = 0; x < BLOCK_SIZE; x++) {
			blocks[x][y] = new Block(BlockType::eEmpty);
			parent->addChild((new Node(Transform(glm::vec3(x, 0, y)), 
				"Block " + std::to_string(x) + " " + std::to_string(y)))
				->addComponent(blocks[x][y]));
		}
	}
}

Node *Field::setBlock(BlockType type, size_t x, size_t y) {

	if(x >= BLOCK_SIZE || y >= BLOCK_SIZE) {
		return nullptr;
	}

	if (blocks[x][y]->getType() != BlockType::eEmpty) {
		removeBlock(x, y);
	}

	if (type != BlockType::eEmpty) {
		blocks[x][y]->setContent(type);
		blocks[x][y]->getParent()->addComponent(
			new RenderComponent(
				parent->getScene()->getMesh("tree.obj"),
				new Material(
					parent->getScene()->getVideoDriver()->getTexture("solis.png"))));
	}

	return blocks[x][y]->getParent();	
}

void Field::removeBlock(size_t x, size_t y) {
	/* return if out of scope or if the block is already empty */
	if (x >= BLOCK_SIZE || y >= BLOCK_SIZE ||
		blocks[x][y]->getType() == BlockType::eEmpty) {
		return;
	}

	parent->removeChild(blocks[x][y]->getParent());

	blocks[x][y] = new Block(BlockType::eEmpty);
	parent->addChild((new Node(Transform(glm::vec3(x, 0, y)), 
		"Block " + std::to_string(x) + " " + std::to_string(y)))
		->addComponent(blocks[x][y]));
}