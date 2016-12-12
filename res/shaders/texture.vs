#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normals;

out vec2 uvs0;

uniform mat4 mvp;

struct Camera {
	vec3 position;
	mat4 viewProjection;
};

struct Transform {
	mat4 model;
};

uniform Camera camera;
uniform Transform transform;

void main() {
	gl_Position = camera.viewProjection * transform.model * vec4(position, 1.0);
	uvs0 = uvs;
}
