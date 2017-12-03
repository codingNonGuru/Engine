#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;
layout(location = 3) uniform float seaLevel;
//layout(location = 5) uniform float time;
layout(location = 6) uniform vec3 skyColor;

layout (location = 0) in vec3 vertPos;

out vec3 position;
out vec3 reflectAngle;

void main()
{
	position = vertPos;
	
	//vec3 reflectAngle = reflect(-lightPos, faceNorm);
	
	//float diffuseFactor = clamp(dot(lightPos, faceNorm), 0.3f, 1.0);
	//color = vec4(diffuseFactor * vertCol, 1.0);
	
	gl_Position = projMatrix * vec4(vertPos, 1.0f);
} 