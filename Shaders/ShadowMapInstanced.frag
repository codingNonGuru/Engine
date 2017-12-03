#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) out float fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.z;
}