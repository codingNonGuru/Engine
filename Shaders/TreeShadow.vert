#version 430

layout (location = 0) uniform mat4 viewProjMatrix;

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec2 vertexTexCoord;

layout (location = 5) in vec3 instancePosition;
layout (location = 6) in float instanceRotation;

out vec2 texCoord;

void main()
{	
	texCoord = vertexTexCoord;

	float s = sin(instanceRotation);
	float c = cos(instanceRotation);
	
	float x = vertexPosition.x * c - vertexPosition.y * s;
	float y = vertexPosition.x * s + vertexPosition.y * c;
	
	vec3 finalPosition = instancePosition + vec3(x, y, vertexPosition.z);
	
	gl_Position = viewProjMatrix * vec4(finalPosition, 1.0f);
} 