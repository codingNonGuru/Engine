#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 viewProjMatrix;
layout (location = 1) uniform vec3 lightDirection;
layout (location = 2) uniform vec3 cameraPosition;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;
layout (location = 2) in float time;

layout (location = 5) in vec3 objectPos;

out vec4 color;
out vec3 pos;
out vec3 normal;

void main() {
	float diffuseFactor = dot(vertNorm, lightDirection);
	//diffuseFactor = clamp(diffuseFactor, 0.4f, 1.0f);
	if(diffuseFactor > 0.0f)
		diffuseFactor = 0.3f + (diffuseFactor * 0.7f);
	else
		diffuseFactor = 0.2f + ((diffuseFactor + 2.0f) * 0.1f);
	
	color.rgb = vec3(1.0f, 0.2f, 0.0f) * diffuseFactor;
	color.a = 1.0f;
	
	vec3 realPos = objectPos + vertPos;
	pos = realPos;
	normal = vertNorm;
	gl_Position = viewProjMatrix * vec4(realPos, 1.0);
}