#pragma once
#include <GL/glew.h>
#include <stdexcept>
#include <map>
#include "videodriver.h"
#include "openglshader.h"
#include "opengltexture.h"

class OpenGLDriver : public VideoDriver {
public:
	// class VBLinkGL : public VBLink{
	// public:
	// 	VBLinkGL(const VertexBuffer *buffer) : VBLink(buffer) {};

	// 	uint32_t vao;
	// 	uint32_t vbo;
	// 	uint32_t ibo;
	// };
	class VBLinkGL : public VBLink {
	public:
		VBLinkGL(std::shared_ptr<const VertexBuffer> buffer) : VBLink(buffer) {};

		uint32_t vao;
		uint32_t vbo;
		uint32_t ibo;
	};
	
	OpenGLDriver(SolisDevice *);
	~OpenGLDriver();

	// VBLink *getBufferLink(const VertexBuffer *) const;
	// VBLink *createBuffer(const VertexBuffer *);
	// void drawVertexBuffer(const VertexBuffer *) const;
	// void drawBufferLink(VBLink *) const;
	// void deleteBuffer(VBLink *);
	VBLink *getBufferLink(std::shared_ptr<const VertexBuffer>) const;
	VBLink *createBuffer(std::shared_ptr<const VertexBuffer>);
	void drawVertexBuffer(std::shared_ptr<const VertexBuffer>) const;
	void drawBufferLink(VBLink *) const;
	void deleteBuffer(VBLink *);

	void addShaderFromFile(const std::string &);
	void bindShader(const std::string &);
	OpenGLShader *getActiveShader() const;
	void deleteShader(const std::string &);

	Texture *getTexture(const std::string &);
	void bindTexture(Texture *) const;

	inline void clearScreenBuffer() { 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};
	
};