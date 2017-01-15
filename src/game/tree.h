#pragma once
#include "blockdata.h"
#include <cstdio>

class Tree : public BlockData {
public:
	Tree(size_t woodAmt = 10) : 
		BlockData(), 
		woodAmt(woodAmt) {};

	int chopWood(size_t amt);
	inline int getWoodAmount() const { return woodAmt; }
	BlockType getType() const override { return BlockType::eTree; };

private:
	size_t woodAmt;
}; 