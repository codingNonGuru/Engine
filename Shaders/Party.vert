#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 projMatrix;
layout (location = 1) uniform mat4 depthMatrix;
layout (location = 2) uniform vec3 lightPos;
layout (location = 3) uniform vec3 cameraPos;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 faceNorm;

layout (location = 5) in vec3 objectPos;
layout (location = 6) in float scale;
layout (location = 7) in float rotation;

//out vec3 objCol;
out vec3 position;
out vec3 normal;
out vec4 shadowCoord;

void main()
{
	float s = sin(rotation);
	float c = cos(rotation);
	
	vec3 faceNormal = vec3(faceNorm.x * c - faceNorm.y * s, faceNorm.x * s + faceNorm.y * c, faceNorm.z);
	normal = faceNormal;
	
	float tempX = vertPos.x * scale;
	float tempY = vertPos.y * scale;
	
	float finalX = tempX * c - tempY * s;
	float finalY = tempX * s + tempY * c;
	
	vec3 realPos = objectPos + vec3(finalX, finalY, vertPos.z * scale);
	position = realPos;
	gl_Position = projMatrix * vec4(realPos, 1.0f);
	shadowCoord = depthMatrix * vec4(realPos, 1.0f);
} 