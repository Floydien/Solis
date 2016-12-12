#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	Vertex() {};
	Vertex(const glm::vec3 &pos = glm::vec3(), const glm::vec2 &uv = glm::vec2(), const glm::vec3 &normal = glm::vec3()) :
		position(pos), uvCoord(uv), normal(normal) {};
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : 
		position(x, y, z), uvCoord(u, v), normal(nx, ny, nz) {};

	
	glm::vec3 position;
	glm::vec2 uvCoord;
	glm::vec3 normal;
};