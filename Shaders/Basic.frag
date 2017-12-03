#version 450

layout (location = 1) uniform vec3 color;

layout (location = 0) out vec4 finalColor;

void main() {
	finalColor.rgba = vec4(1.0f);
}