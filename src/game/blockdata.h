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
	Building(const glm::vec3 &extent): BlockData(), extent(extent) {};

	inline BlockType getType() const override { return BlockType::eBuilding; };
	virtual inline BuildingType getBuildingType() const = 0;	

	inline const glm::vec3 &getExtent() { return extent; };
protected:
	glm::vec3 extent;
};

class Sawmill : public Building {
public:
	Sawmill() : Building({2.0, 1.0, 1.0}) {};
	inline BuildingType getBuildingType() const override { return BuildingType::eSawmill; };
};
