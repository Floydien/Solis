#pragma once
#include "field.h"
#include "nodecomponent.h"
#include <glm/glm.hpp>


class Player : public NodeComponent {
public:
	Player(Field *field) : 
		field(field) {};
	virtual ~Player() {};

	virtual void init();
	virtual void input(float, SolisDevice *);
	virtual void update(float);
	virtual void render(const VideoDriver *) const {};
private:
	Field *field;
	glm::vec3 prevPosition;
};