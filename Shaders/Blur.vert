#version 450

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec2 scale;
layout(location = 2) uniform vec2 displacement;

layout (location = 0) in vec3 vertPos;

void main() {
	gl_Position = projMatrix * vec4(vertPos.x * scale.x + displacement.x, vertPos.y * scale.y + displacement.y, 0.5f, 1.0f);
}