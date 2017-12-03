#version 450

layout (location = 0) out vec4 finalColor;

in float color;

void main() {
	finalColor = vec4(1.0f, 1.0f, 1.0f, color);
}
