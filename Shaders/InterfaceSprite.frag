#version 330
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D alphaMap;

layout (location = 0) out vec4 finalColor;

in vec2 texCoord;

void main() {
	float alpha = texture(alphaMap, texCoord).r;
	/*if(alpha < 0.5f) {
		int y = int(texCoord.y * 128.0f);
		alpha = y % 4 == 0 ? 0.35f : 0.0f;
	}*/
	finalColor.rgba = vec4(0.75f, 0.0f, 0.2f, alpha);
}