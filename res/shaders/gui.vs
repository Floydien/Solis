#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uvs;

out vec2 uvs0;

uniform ivec2 windowSize;

void main() {
	gl_Position = vec4((position.x / windowSize.x) * 2.0 - 1,
				1.0 - (position.y / windowSize.y) * 2,
				0.0, 1.0);
	uvs0 = uvs;
}
