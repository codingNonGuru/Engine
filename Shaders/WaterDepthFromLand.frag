#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) out vec4 finalColor;
layout(location = 1) uniform vec3 lightPos;
uniform sampler2D normalMap;

in vec4 color;
in vec3 position;
in vec3 normal;
in vec3 reflectAngle;
in vec3 camera;

void main()
{
	vec3 normal;
	normal.xyz = texture(normalMap, position.xy * 0.05f).rgb;
	
	vec3 cameraRay = normalize(camera - position);
	vec4 refAngle = reflect(-vec4(lightPos, 0.0f), vec4(normalize(normal), 0.0f));
	float specFactor = clamp(dot(refAngle, vec4(cameraRay, 0.0f)), 0.0, 1.0f);
	specFactor = 1 - exp(-pow(specFactor, 50) / .5);
	float difFactor = clamp(dot(lightPos, normalize(normal)), 0.3f, 1.0f);
	
	float Fac = 1.8f - gl_FragCoord.z;
	vec4 airColor = vec4(0.258, 0.656, 0.97, 1.0);
	finalColor.xyz = difFactor * color.xyz;
	finalColor.xyz = specFactor + finalColor.xyz;
	finalColor.w = 1.0f;
	finalColor.xyz = Fac * finalColor.xyz;
	finalColor.xyz = finalColor.xyz + ((1 - Fac) * airColor.xyz);
	//finalColor.w = 1.0;
} 