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
	BlockData(BlockType type) : type(type) {};

	inline BlockType getType() { return type; };

private:
	const BlockType type;		
};

class EmptyBlock : public BlockData {
public:
	EmptyBlock() : BlockData(BlockType::eEmpty) {};
};

class DirtBlock : public BlockData {
public:
	DirtBlock() : BlockData(BlockType::eDirt) {};
};

class Tree : public BlockData {
public:
	Tree() :BlockData(BlockType::eTree) {};
};

class Building : public BlockData {
public:
	Building(const glm::vec3 &halfExtent): BlockData(BlockType::eBuilding), halfExtent(halfExtent) {};
	//building type
	inline const glm::vec3 &getHalfExtent() { return halfExtent; };

protected:
	glm::vec3 halfExtent;
};

class Sawmill : public Building {
public:
	Sawmill() : Building({1.0, 0.5, 1.0}) {};
};
