#include "opengltexture.h"

OpenGLTexture::OpenGLTexture(const std::string &fileName) : Texture(fileName) {
    name = fileName;

    Image img(fileName);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0,
                GL_RGBA, GL_UNSIGNED_BYTE, img.getData().data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &textureID);
}

void OpenGLTexture::bind(uint32_t location) const {
    glActiveTexture(location);
    glBindTexture(GL_TEXTURE_2D, textureID);
}