#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) uniform mat4 projMatrix;

layout (location = 0) in vec3 landPos;
layout (location = 1) in vec3 waterPos;

out vec3 lPos;
out vec3 wPos;

void main()
{
	wPos = projMatrix * vec3(waterPos, 1.0f);
	
	gl_Position = projMatrix * vec4(landPos, 1.0);
} 