#pragma once
#include <cstring>
#include <map>
#include <memory>
#include "vertexbuffer.h"

class Mesh {
public:
	Mesh(const std::string &);
	Mesh(const std::string &, VertexBuffer *);
	/** 
	 * If a VertexBuffer with the same name is already loaded, the mesh will use the already loaded buffer,
	 * else it uses the one passed by parameter
	 */
	Mesh(const std::string &, std::shared_ptr<VertexBuffer>);
	~Mesh();

	// inline const VertexBuffer *getVertexBuffer() const { return buffer.get(); };
	inline std::shared_ptr<VertexBuffer> getVertexBuffer() const { return buffer; };

private:
	std::string name;
	std::shared_ptr<VertexBuffer> buffer;

	static std::map<const std::string, std::shared_ptr<VertexBuffer>> vbufferMap;
};