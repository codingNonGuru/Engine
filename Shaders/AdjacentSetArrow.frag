#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 1) uniform vec3 lightDirection;
layout (location = 2) uniform vec3 cameraPosition;

layout (location = 0) out vec4 finalColor;

in vec4 color;
in vec3 pos;
in vec3 normal;

void main() {
	vec4 Light = vec4(lightDirection, 0.0);
	vec3 cameraRay = cameraPosition - pos;
	cameraRay = normalize(cameraRay);
	
	vec4 refAngle = reflect(-Light, vec4(normal, 0.0));
	float specIntensity = clamp(dot(refAngle, vec4(cameraRay, 0.0)), 0.0, 1.0);
	specIntensity = 1.0f - exp(-pow(specIntensity, 4.0f) / 0.5f);
	specIntensity *= 0.7f;
	
	finalColor.rgb = color.rgb + specIntensity;
	finalColor.a = 1.0f;
}
