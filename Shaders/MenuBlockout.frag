#version 450
//#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;
layout(location = 3) uniform float depth;

in vec2 position;

uniform sampler2D diffuse;

layout (location = 0) out float fragmentDepth;

void main() {
	fragmentDepth = texture(diffuse, position).a;
	//fragmentDepth *= opacity;
}
