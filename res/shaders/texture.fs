#version 330 core

in vec2 uvs0;

out vec4 colorOut;

uniform sampler2D textureData;

void main() {
	colorOut = texture(textureData, uvs0);
}