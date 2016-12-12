#pragma once
#include <array>
#include "block.h"

class Field {
public:
	Field() {};

	
	
	std::array<std::array<Block, 32>, 32> blocks;
private:
};