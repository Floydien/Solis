#pragma once
#include "blockdata.h"
#include <memory>

class Block {
public:
	Block(std::shared_ptr<BlockData> content) : content(content) {};
	Block(BlockType type) { setContent(type); };
	Block() {
		content = EMPTY_BLOCK;
	};

	inline void setContent(std::shared_ptr<BlockData> content) { this->content = content; };
	void setContent(BlockType);

	inline std::shared_ptr<BlockData> getContent() { return content; };
	inline BlockType getType() const { return content->getType(); };	

	static const std::shared_ptr<EmptyBlock> EMPTY_BLOCK;
private:
	std::shared_ptr<BlockData> content;
};
