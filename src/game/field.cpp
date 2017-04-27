#include "field.h"
#include "rendercomponent.h"

std::shared_ptr<VertexBuffer> generateField(uint32_t size) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 position;
	glm::vec2 uvCoord;
	glm::vec3 normal;

	for(size_t i = 0; i < size + 1; i++) {
		for(size_t k = 0; k < size + 1; k++) {
			position = {i, 0, k};

			uvCoord = {(float) (i % size) , (float) (k % size)};
			normal = {0, 1, 0};
			vertices.emplace_back(position, uvCoord, normal);
		}
	}

	for(size_t i = 0; i < size * size; i++) {
		indices.push_back((i + i / size));
		indices.push_back((i + i / size) + 1);
		indices.push_back((i + i / size) + (size + 1));

		indices.push_back((i + i / size) + 1);
		indices.push_back((i + i / size) + (size + 2));
		indices.push_back((i + i / size) + (size + 1));
	}

	return std::make_shared<VertexBuffer>(vertices, indices);
}

void Field::init() {
	parent->addComponent((new RenderComponent(
			parent->getScene()->getMesh("field", generateField(32)),
			new Material(parent->getScene()->getVideoDriver()->getTexture("solis.png")))));

	for (size_t y = 0; y < FIELD_SIZE; y++) {
		for (size_t x = 0; x < FIELD_SIZE; x++) {
			blocks.at(x).at(y) = new Block(BlockType::eEmpty);
			parent->addChild((new Node(Transform(glm::vec3(x, 0, y)), 
				"Block " + std::to_string(x) + " " + std::to_string(y)))
				->addComponent(blocks.at(x).at(y)));
		}
	}
}

Node *Field::setBlock(BlockType type, size_t x, size_t y) {
	if(x >= FIELD_SIZE || y >= FIELD_SIZE) {
		return nullptr;
	}

	if (blocks.at(x).at(y)->getType() != BlockType::eEmpty) {
		removeBlock(x, y);
	}

	if (type != BlockType::eEmpty) {
		blocks.at(x).at(y)->setContent(type);
		switch (type) {
			case BlockType::eTree :
				blocks.at(x).at(y)->getParent()->addComponent(
					new RenderComponent(
						parent->getScene()->getMesh("tree.obj"),
						new Material(
							parent->getScene()->getVideoDriver()->getTexture("solis.png"))));
				break;
			default : 
				break;
		}
	}

	return blocks.at(x).at(y)->getParent();	
}

void Field::removeBlock(size_t x, size_t y) {
	/* return if out of scope or if the block is already empty */
	if (x >= FIELD_SIZE || y >= FIELD_SIZE ||
		blocks.at(x).at(y)->getType() == BlockType::eEmpty) {
		return;
	}

	parent->removeChild(blocks[x][y]->getParent());

	blocks.at(x).at(y) = new Block(BlockType::eEmpty);
	parent->addChild((new Node(Transform(glm::vec3(x, 0, y)), 
		"Block " + std::to_string(x) + " " + std::to_string(y)))
		->addComponent(blocks.at(x).at(y)));
}

bool Field::build(BuildingType type, size_t x, size_t z) {
	/* Create the building */
	std::shared_ptr<Building> building;
	switch (type) {
		case BuildingType::eSawmill : 
			building = std::make_shared<Sawmill>();
			break;
		default :
			break;
	}

	/* check, whether the building is outside of the field or something is blocking it */
	auto &&extent = building->getExtent();
	size_t extentX = x + extent.x;
	size_t extentZ = z + extent.z;

	if (extentX >= FIELD_SIZE || extentZ >= FIELD_SIZE) {
		return false;
	}

	for (size_t _x = x; _x < extentX; _x++) {
		for (size_t _z = z; _z < extentZ; _z++) {
			if (blocks.at(_x).at(_z)->getType() != BlockType::eEmpty) {
				return false;
			}
		}
	}

	/* set the blocks of the field to the building */
	for (size_t _x = x; _x < extentX; _x++) {
		for (size_t _z = z; _z < extentZ; _z++) {
			blocks.at(_x).at(_z)->setContent(building);
		}
	}

	blocks.at(x).at(z)->getParent()->addComponent(
		new RenderComponent(
			parent->getScene()->getMesh("sawmill.obj"),
			new Material(
				parent->getScene()->getVideoDriver()->getTexture("solis.png"))));

	return true;
}