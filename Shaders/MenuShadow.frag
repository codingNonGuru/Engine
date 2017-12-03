#version 450
//#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;
layout(location = 4) uniform float opacity;

in vec2 texCoord;
in vec2 position;

uniform sampler2D shadow;
uniform sampler2D menuBlockout;

layout (location = 0) out vec4 finalColor;

void main() {
	float menuAlpha = texture(menuBlockout, position).r;
	finalColor.rgb = vec3(0.0f);
	finalColor.a = texture(shadow, texCoord).r * menuAlpha;
}
