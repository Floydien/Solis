#pragma once
#include "nodecomponent.h"
#include "blockdata.h"
#include <memory>

class Block : public NodeComponent {
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

	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};

	/* static pointer to which content will point if it is of the type eEmpty */
	static const std::shared_ptr<EmptyBlock> EMPTY_BLOCK;
private:
	std::shared_ptr<BlockData> content;
};
