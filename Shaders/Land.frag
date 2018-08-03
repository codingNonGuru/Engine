#version 450

layout(location = 1) uniform mat4 depthMatrix;
layout(location = 2) uniform vec3 lightPos;
layout(location = 3) uniform vec3 cameraPos;
layout(location = 7) uniform float seaLevel;
layout(location = 8) uniform vec2 specialTile;
layout(location = 10) uniform vec2 stencilPosition;
layout(location = 11) uniform vec2 stencilScale;

in vec3 pos;
in vec4 shadowCoord;
in float desFactor;
in vec3 light;
in vec3 eye;
in vec3 normal;

layout (location = 0) out vec4 finalColor;

float atan2(float y, float x) 
{
	float angle = atan(y / x);
	if(x > 0)
		return angle;
	if(x < 0 && y >= 0)
		return angle + 3.1415f;
	if(x < 0 && y < 0)
		return angle - 3.1415f;
	if(x == 0 && y > 0)
		return 1.57079f;
	if(x == 0 && y < 0)
		return -1.57079f;
}

void main() 
{
	vec3 lightDirection = normalize(light);

	float diffuse = dot(vec3(0.0f, 0.0f, 1.0f), lightDirection);
	if(diffuse < 0.0f)
		diffuse = 0.0f;

	vec4 refAngle = reflect(-vec4(lightDirection, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.0f));
	vec3 cameraRay = normalize(eye);
	float specular = dot(refAngle, vec4(cameraRay, 0.0f));
	if(specular < 0.0f)
		specular = 0.0f;

	specular = pow(specular, 256.0f) * 0.5f + pow(specular, 64.0f) * 0.25f + pow(specular, 16.0f) * 0.25f;
	//specular *= 0.5f;
	
	vec3 color = vec3(pos.z < 0.0f ? 1.0f : 0.5f) * diffuse;
	color = color * (1.0f - specular) + vec3(specular);

	finalColor = vec4(color.rgb, 1.0f);
	//finalColor = vec4(1.0f);
}