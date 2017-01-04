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
	VideoDriver(SolisDevice *device) : 
		device(device),
		activeShader(nullptr) {};
	virtual ~VideoDriver() = default;

	virtual VBLink *getBufferLink(const VertexBuffer *) const = 0;
	virtual VBLink *createBuffer(const VertexBuffer *) = 0;
	virtual void drawVertexBuffer(const VertexBuffer *) const = 0;
	virtual void drawBufferLink(VBLink *) const = 0;
	virtual void deleteBuffer(VBLink *) = 0;

	virtual void addShaderFromFile(const std::string &) = 0;
	virtual Shader *getActiveShader() const = 0;
	virtual void deleteShader() = 0;

	virtual Texture *getTexture(const std::string &) = 0;
	virtual void bindTexture(Texture *) const = 0;

	virtual void clearScreenBuffer() = 0;
protected:
	SolisDevice *device;
	Shader *activeShader;

	std::map<const VertexBuffer *, VBLink *> vertexBufferMap;
	std::map<const std::string, Texture *> textureMap;

};