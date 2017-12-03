#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;
layout(location = 3) uniform mat4 depthMatrix;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 faceNorm;
layout (location = 2) in vec2 vertCoord;
layout (location = 5) in vec3 objectPos;
layout (location = 6) in vec3 objectCol;

out vec3 position;
out vec3 faceNormal;
out vec3 color;
out vec4 shadowCoord;
out vec2 texCoord;

void main()
{
	vec3 realPos = vertPos + objectPos;
	shadowCoord = depthMatrix * vec4(realPos, 1.0);
	//float difFactor = clamp(dot(lightPos, faceNorm), 0.2f, 1.0f);
	//vec3 difColor = difFactor * objectCol;
	
	float side = dot(cameraPos - realPos, faceNorm);
	
	if(side < 0.0f)
		faceNormal = -faceNorm;
	else
		faceNormal = faceNorm;
	
	//vec4 Light = vec4(lightPos, 0.0);
	//vec3 cameraRay = cameraPos - realPos;
	//cameraRay = normalize(cameraRay);
	
	//vec4 refAngle = reflect(-Light, vec4(faceNorm, 0.0));
	//float specIntensity = clamp(dot(refAngle, vec4(cameraRay, 0.0)), 0.0, 1.0);
	
	//difColor = clamp(specIntensity * 0.33, 0.0, 0.3) + difColor;
	color = objectCol;
	texCoord = vertCoord;
	
	position = realPos;
	vec4 V = vec4(realPos, 1.0);
	gl_Position = projMatrix * V;
} 