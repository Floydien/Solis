#pragma once
#include <cstring>
#include <map>
#include "vertexbuffer.h"

class Mesh {
public:
	Mesh(const std::string &);
	Mesh(const std::string &, VertexBuffer *);
	~Mesh();

	inline const VertexBuffer *getVertexBuffer() const { return buffer; };
private:
	std::string name;
	VertexBuffer *buffer;

	static std::map<const std::string, VertexBuffer *> vbufferMap;
};