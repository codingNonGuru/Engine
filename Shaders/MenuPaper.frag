#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;
layout(location = 4) uniform float opacity;

in vec2 texCoord;

uniform sampler2D diffuse;

layout (location = 0) out vec4 finalColor;

void main() {
	/*finalColor.rgba = texture(diffuse, texCoord).rgba;
	//finalColor.a *= 1000.0f;
	//finalColor.a = 1.0f;
	
	if(finalColor.a > 0.2f)
		finalColor.a = 1.0f;
	else
		finalColor.a = 0.0f;
		
	finalColor.a *= opacity;*/
	finalColor.rgba = texture(diffuse, texCoord).rgba;
	//finalColor.a = finalColor.a > 0.5f ? 1.0f : 0.0f;
	//finalColor.a *= 0.5f;
	//finalColor.rgba = vec4(1.0f, 0.0f, 1.0f, 1.0f);
} 