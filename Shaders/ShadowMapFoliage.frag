#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) out float fragmentDepth;

uniform sampler2D foliageAlpha;

in vec2 texCoord;

void main() {
	float alpha = texture(foliageAlpha, texCoord).a;
	if(alpha < 0.5f)
		discard;
	fragmentDepth = gl_FragCoord.z;
}