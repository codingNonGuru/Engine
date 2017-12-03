#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 viewProjMatrix;

layout (location = 0) in vec3 vertPos;

layout (location = 5) in vec3 objectPos;

void main()
{
	vec3 realPos = objectPos + vertPos;
	gl_Position = viewProjMatrix * vec4(realPos, 1.0);
} 