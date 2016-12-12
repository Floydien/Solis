#pragma once
#include <string.h>
#include <map>
#include "vertexbuffer.h"

class Mesh {
public:
	Mesh(const std::string &);
	Mesh(const std::string &, VertexBuffer *);
	~Mesh();

	inline const VertexBuffer *getVertexBuffer() const { return buffer; };
private:
	VertexBuffer *buffer;

	static std::map<const std::string, VertexBuffer *> vbufferMap;
};