#pragma once
#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "vertexbuffer.h"
#include "texture.h"
#include "shader.h"

class SolisDevice;

class VideoDriver {
protected:
	class VBLink {
	public:
		VBLink(const VertexBuffer *buffer) : vertexBuffer(buffer) {};

		const VertexBuffer *vertexBuffer;
	};
public:
	VideoDriver(SolisDevice *device) : device(device) {};
	virtual ~VideoDriver() {};

	virtual VBLink *getBufferLink(const VertexBuffer *) const { return nullptr; };
	virtual VBLink *createBuffer(const VertexBuffer *) { return nullptr; };
	virtual void drawVertexBuffer(const VertexBuffer *) const {};
	virtual void drawBufferLink(VBLink *) const {};
	virtual void deleteBuffer(VBLink *) {};

	virtual void addShaderFromFile(const std::string &) { };
	virtual Shader *getActiveShader() const { return nullptr; };
	virtual void deleteShader() {};

	virtual Texture *getTexture(const std::string &) { return nullptr; }
	virtual void bindTexture(Texture *) const {};

	virtual void clearScreenBuffer() {};
protected:
	SolisDevice *device;
	Shader *activeShader;

	std::map<const VertexBuffer *, VBLink *> vertexBufferMap;
	std::map<const std::string, Texture *> textureMap;

};