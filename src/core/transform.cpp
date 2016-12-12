#include "transform.h"

#include <glm/vec4.hpp>
#include <stdio.h>

glm::mat4 Transform::getTransformation() const {
	return getParentMatrix() *
			glm::translate(position) *
			glm::toMat4(rotation) *
			glm::scale(scale);
}

const glm::mat4& Transform::getParentMatrix() const {
	if(parent != 0)
		parentMatrix = parent->getTransformation();
	return parentMatrix;
}

glm::vec3 Transform::getTransformedPosition() const {
	glm::vec4 v(position, 1);
	v = getParentMatrix() * v;
	return glm::vec3(v.x, v.y, v.z);
}

glm::quat Transform::getTransformedRotation() const {
	glm::quat rot(0,0,0,1);
	if(parent != 0)
		rot = parent->getTransformedRotation();
	return rotation * rot;
}

void Transform::rotate(const glm::quat& rotation) {
	this->rotation *= rotation;
}

void Transform::rotate(const glm::vec3& axis, float angle) {
	rotation = glm::normalize(glm::rotate(rotation, angle, axis));
}

void Transform::moveForward(float amount) {
	position += rotation * glm::vec3(0,0,-1) * amount;
}

void Transform::moveRight(float amount) {
	position += rotation * glm::vec3(1,0,0) * amount;
}

glm::vec3 Transform::getRight() const {
	return rotation * glm::vec3(1,0,0);
}

glm::vec3 Transform::getForward() const {
	return rotation * glm::vec3(0,0,1);
}

void Transform::move(const glm::vec3& direction, float amount) {
	position += glm::normalize(direction) * amount;
}
