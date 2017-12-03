#version 450
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 1) uniform vec3 cameraPos;
layout(location = 2) uniform vec3 skyColor;
//uniform sampler2D heightMap;

in vec3 pos;

layout (location = 0) out vec4 finalColor;

void main() {
	vec3 cameraRay = pos - cameraPos;
	cameraRay = normalize(cameraRay);
	
	float skyHeight = asin(cameraRay.z) / 1.57079f;
	if(skyHeight < 0.0f)
		skyHeight = 0.0f;
	skyHeight = pow(skyHeight, 2.0f);
	//skyHeight = 1.0f - exp(-pow(skyHeight, 2.0f) / 0.1f);
	
	finalColor.rgb = skyColor.xyz * skyHeight + vec3(1.0f - skyHeight);
	finalColor.a = 1.0f;
}