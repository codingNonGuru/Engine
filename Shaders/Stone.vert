#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;
layout(location = 3) uniform mat4 depthMatrix;
layout(location = 4) uniform uint targetSettlement;
layout(location = 10) uniform vec2 stencilPosition;
layout(location = 11) uniform vec2 stencilScale;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 faceNorm;
layout (location = 2) in vec3 faceTangent;
layout (location = 3) in vec3 faceBitangent;
layout (location = 4) in vec2 texCoords;

layout (location = 5) in vec3 objectPos;
layout (location = 6) in float scale;
layout (location = 7) in float rotation;
layout (location = 8) in uint settlement;

out vec3 faceNormal;
out vec3 position;
out vec3 coords;
out vec4 shadowCoord;
out vec3 tangentSpaceLight;
out vec3 tangentSpaceEye;
out float highlight;

uniform sampler2D pavementStencil;

void main() {
	float s = sin(rotation);
	float c = cos(rotation);
	
	faceNormal = vec3(faceNorm.x * c - faceNorm.y * s, faceNorm.x * s + faceNorm.y * c, faceNorm.z);
	vec3 tangent = vec3(faceTangent.x * c - faceTangent.y * s, faceTangent.x * s + faceTangent.y * c, faceTangent.z);
	vec3 bitangent = vec3(faceBitangent.x * c - faceBitangent.y * s, faceBitangent.x * s + faceBitangent.y * c, faceBitangent.z);
	
	float x = vertPos.x * c - vertPos.y * s;
	float y = vertPos.x * s + vertPos.y * c;
	
	vec2 stencilCoord = objectPos.xy - stencilPosition.xy;
	stencilCoord.x /= stencilScale.x;
	stencilCoord.y /= stencilScale.y;
	float heightOffset = texture(pavementStencil, stencilCoord).r * 0.05f;
	
	vec3 realPos = objectPos + vec3(x, y, vertPos.z + heightOffset);
	position = realPos;
	coords = vec3(texCoords.xy, 0.0f);
	gl_Position = projMatrix * vec4(realPos, 1.0);
	shadowCoord = depthMatrix * vec4(realPos, 1.0f);
	
	mat3 tbn = transpose(mat3(tangent, bitangent, faceNormal));
	tangentSpaceLight = tbn * (inverse(mat4(1.0f)) * vec4(lightPos, 0.0f)).xyz;
	tangentSpaceEye = tbn * (cameraPos - realPos);
	
	if(targetSettlement == settlement)
		highlight = 0.8f;
	else
		highlight = 1.0f;
} 