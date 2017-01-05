#include "block.h"

const std::shared_ptr<EmptyBlock> Block::EMPTY_BLOCK = std::make_shared<EmptyBlock>();

void Block::setContent(BlockType type) {
	switch (type) {
		case BlockType::eEmpty : 
			content = EMPTY_BLOCK;
			break;
		case BlockType::eTree :
			content = std::make_shared<Tree>();
			break;
		case BlockType::eDirt :
			content = std::make_shared<DirtBlock>();
			break;
		case BlockType::eBuilding :
			/* TODO: choose a building*/
			break;
	}
}