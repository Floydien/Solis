#include "opengldriver.h"

OpenGLDriver::OpenGLDriver(SolisDevice *device) : 
        VideoDriver(device) {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
    	throw "Error: Could not init glew";
    }
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
}

OpenGLDriver::~OpenGLDriver() {
    while(vertexBufferMap.size()) {
        deleteBuffer(vertexBufferMap.begin()->second);
    }

    while(textureMap.size()) {
        auto it = textureMap.begin();
        if(it->second)
            delete it->second;
        textureMap.erase(it);
    }
}

OpenGLDriver::VBLink *OpenGLDriver::getBufferLink(const VertexBuffer *vb) const {
    if(!vb) {
        return nullptr;
    }
    auto it = vertexBufferMap.find(vb);

    if(it != vertexBufferMap.end()) {
        return it->second;
    }

    return nullptr;
}

OpenGLDriver::VBLink *OpenGLDriver::createBuffer(const VertexBuffer *vb) {
	if(!vb) {
		return nullptr;
	}
	auto it = vertexBufferMap.find(vb);
	if(it != vertexBufferMap.end()) {
		return it->second;
	}

	VBLinkGL *vblink = new VBLinkGL(vb);

    glGenVertexArrays(1, &vblink->vao);
    glBindVertexArray(vblink->vao);

    glGenBuffers(1, &vblink->vbo);
    glGenBuffers(1, &vblink->ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vblink->vbo);
    glBufferData(GL_ARRAY_BUFFER, 
        vb->getVertices().size() * sizeof(vb->getVertices()[0]), 
        vb->getVertices().data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vb->getVertices()[0]), 0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vb->getVertices()[0]), (GLvoid*) (sizeof(float)*3));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vb->getVertices()[0]), (GLvoid*) (sizeof(float)*5));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vblink->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        vb->getVertexIndices().size() * sizeof(vb->getVertexIndices()[0]),
        vb->getVertexIndices().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    vertexBufferMap.insert(std::pair<const VertexBuffer *, VBLink *>(vb, vblink));

	return vblink;
}

void OpenGLDriver::deleteBuffer(OpenGLDriver::VBLink *bufferLink) {
	auto it = vertexBufferMap.find(bufferLink->vertexBuffer);
	if(it != vertexBufferMap.end()) {
    	glDeleteVertexArrays(1, &static_cast<VBLinkGL *>(it->second)->vao);
    	glDeleteBuffers(1, &static_cast<VBLinkGL *>(it->second)->vbo);
    	glDeleteBuffers(1, &static_cast<VBLinkGL *>(it->second)->ibo);

        delete it->second;
    	vertexBufferMap.erase(it);
	}
}

void OpenGLDriver::drawBufferLink(OpenGLDriver::VBLink *bufferLink) const {
	if(!bufferLink) {
		return;
	}

    glBindVertexArray(static_cast<VBLinkGL *>(bufferLink)->vao);
    glDrawElements(GL_TRIANGLES, static_cast<VBLinkGL *>(bufferLink)->vertexBuffer->getVertexIndices().size(), GL_UNSIGNED_INT, 0);
}

void OpenGLDriver::drawVertexBuffer(const VertexBuffer *vbuffer) const {
    if(!vbuffer) {
        return;
    }

    auto bufferLink = getBufferLink(vbuffer);

    if(bufferLink) {
        drawBufferLink(bufferLink);
    }
}


void OpenGLDriver::addShaderFromFile(const std::string &name) {
    auto it = activeShaders.find(name);
    if (it != activeShaders.end()) {
        activeShader = it->second;
    } else {
        activeShader = std::make_shared<OpenGLShader>(name);//new OpenGLShader(filename); 
        activeShaders.insert(std::pair<const std::string, std::shared_ptr<Shader>>(name, activeShader));
    }
}

void OpenGLDriver::bindShader(const std::string &name) {
    auto it = activeShaders.find(name);
    if (it != activeShaders.end()) {
        activeShader = it->second;
    }
    activeShader->bind();
}

OpenGLShader *OpenGLDriver::getActiveShader() const{ 
    return static_cast<OpenGLShader*>(activeShader.get()); 
};

void OpenGLDriver::deleteShader(const std::string &name) {
    auto it = activeShaders.find(name);
    if (it != activeShaders.end()) {
        if (it->second == activeShader) {
            activeShader.reset();
        }
        activeShaders.erase(it);
    }
}

Texture *OpenGLDriver::getTexture(const std::string &filename) {
    auto it = textureMap.find(filename);
    if(it != textureMap.end()) {
        return it->second;
    }
    OpenGLTexture *texture = new OpenGLTexture(filename);

    if(texture) {
        textureMap.insert(std::pair<const std::string, Texture *>(filename, texture));
        return texture;
    }

    return nullptr;
}

void OpenGLDriver::bindTexture(Texture *texture) const{
    texture->bind(0);
}
