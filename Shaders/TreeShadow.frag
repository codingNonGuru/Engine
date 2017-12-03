#version 430

layout (location = 0) out float fragmentDepth;

uniform sampler2D foliageDiffuse;

in vec2 texCoord;

void main() {
	fragmentDepth = gl_FragCoord.z;
}