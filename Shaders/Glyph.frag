#version 330
#extension GL_ARB_explicit_uniform_location : enable

in vec2 texCoord;
in vec4 color;

uniform sampler2D fontAlpha;

layout (location = 0) out vec4 finalColor;

void main() {
	finalColor.rgb = color.rgb;
	finalColor.a = texture(fontAlpha, texCoord).r * color.a;
} 