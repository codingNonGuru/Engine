#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 viewProjMatrix;

layout (location = 0) in vec3 vertPos;

layout (location = 5) in vec3 objectPos;
layout (location = 6) in float scale;
layout (location = 8) in float rotation;

void main()
{
	float s = sin(rotation);
	float c = cos(rotation);
	
	float tempX = vertPos.x * scale;
	float tempY = vertPos.y * scale;
	
	float finalX = tempX * c - tempY * s;
	float finalY = tempX * s + tempY * c;
	
	vec3 realPos = objectPos + vec3(finalX, finalY, vertPos.z * scale);
	
	gl_Position = viewProjMatrix * vec4(realPos, 1.0);
} 