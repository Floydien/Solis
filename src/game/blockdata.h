#pragma once
#include <glm/glm.hpp>

enum class BlockType {
	eEmpty,
	eDirt,
	eTree,
	eBuilding,
};

enum class BuildingType {
	eSawmill,
};

class BlockData {
public:
	//type of block. eg Dirt, Buildings, trees, etc
	BlockData() = default;

	virtual inline BlockType getType() const = 0;	
};

class EmptyBlock : public BlockData {
public:
	EmptyBlock() = default;
	inline BlockType getType() const override { return BlockType::eEmpty; };
};

class DirtBlock : public BlockData {
public:
	DirtBlock() = default;
	inline BlockType getType() const override { return BlockType::eDirt; };
};

class Building : public BlockData {
public:
	Building(const glm::vec3 &halfExtent): BlockData(), halfExtent(halfExtent) {};
	//building type
	inline const glm::vec3 &getHalfExtent() { return halfExtent; };
	inline BlockType getType() const override { return BlockType::eBuilding; };

protected:
	glm::vec3 halfExtent;
};

class Sawmill : public Building {
public:
	Sawmill() : Building({1.0, 0.5, 1.0}) {};
};
