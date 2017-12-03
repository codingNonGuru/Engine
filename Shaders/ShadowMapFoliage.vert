#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 viewProjMatrix;

layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec2 vertCoord;

layout (location = 5) in vec3 objectPos;

out vec2 texCoord;

void main()
{
	texCoord = vertCoord;
	
	vec3 realPos = objectPos + vertPos;
	gl_Position = viewProjMatrix * vec4(realPos, 1.0);
} 