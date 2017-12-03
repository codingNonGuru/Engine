#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec3 cameraPos;
layout(location = 2) uniform vec3 bgroundCol;
layout(location = 3) uniform float seaLevel;

layout (location = 0) in vec3 vertPos;

out vec3 pos;

void main() {
	gl_Position = projMatrix * vec4(vertPos, 1.0);
	pos = vertPos;
}