#version 450

layout (location = 0) uniform mat4 viewProjMatrix;

layout (location = 0) in vec3 vertexPosition;

layout (location = 5) in vec3 instancePosition;
layout (location = 6) in float instanceScale;
layout (location = 7) in float instanceRotation;

void main() {	
	float s = sin(instanceRotation) * instanceScale;
	float c = cos(instanceRotation) * instanceScale;
	
	float x = vertexPosition.x * c - vertexPosition.y * s;
	float y = vertexPosition.x * s + vertexPosition.y * c;
	
	vec3 finalPosition = instancePosition + vec3(x, y, vertexPosition.z * instanceScale);
	
	gl_Position = viewProjMatrix * vec4(finalPosition, 1.0f);
} 