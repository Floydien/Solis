#pragma once
#include "field.h"
#include "nodecomponent.h"
#include <glm/glm.hpp>


class Player : public NodeComponent {
public:
	Player(Field *field) :
		NodeComponent(),
		field(field),
		carrying(0) {};
	~Player() override = default;

	void init() override;
	void input(float, SolisDevice *) override;
	void update(float) override;
	void render(const VideoDriver *) const override {};
	
private:
	static const size_t MAX_CARRY_AMOUNT;

	Field *field;
	glm::vec3 prevPosition;

	size_t carrying;
};