#version 430
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;

layout (location = 0) out vec4 finalColor;

uniform sampler2D shadowMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

in vec3 faceNormal;
in vec3 position;
in vec3 coords;
in vec4 shadowCoord;
in vec3 tangentSpaceLight;
in vec3 tangentSpaceEye;
in float highlight;

float computeShadow(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float bias = 0.000001f;
	
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 0.0f : 1.0f;        
		}    
	shadow /= 9.0f;

    return shadow;
}

void main()
{
	vec4 color = texture(diffuseMap, coords.xy).rgba;

	vec3 cameraRay = normalize(tangentSpaceEye);
	cameraRay = normalize(cameraRay);
	vec3 faceNorm = normalize(faceNormal);
	vec3 pixelNorm = texture(normalMap, coords.xy).rgb;
	pixelNorm = pixelNorm * 2.0f - 1.0f;
	pixelNorm = normalize(pixelNorm);
	faceNorm = pixelNorm;
	
	float shadow = computeShadow(shadowCoord);
	//shadow = shadow * 0.9f + 0.1f;
	
	float fresnel = dot(cameraRay, faceNorm);
	if(fresnel < 0.0f)
		fresnel = 0.0f;
	fresnel = 1.0f - fresnel;
	fresnel = pow(fresnel, pow(2.0f, 7.0f - 8.0f * fresnel));
	fresnel *= shadow * 0.5f + 0.5f;
	fresnel *= color.a * 0.2f;
	
	float diffuse = dot(tangentSpaceLight, faceNorm);
	bool isDark = false;
	if(diffuse < 0.0f) {
		diffuse = 0.0f;
		isDark = true;
	}
	if(shadow < diffuse && isDark == false) {
		diffuse *= shadow;
		diffuse = diffuse * 0.8f + 0.2f;
	}
	else if(isDark == true)
		diffuse = diffuse * 0.75f + 0.25f;
	else
		diffuse = diffuse * 0.75f + 0.25f;
	
	float specular = clamp(dot(reflect(-tangentSpaceLight, faceNorm), cameraRay), 0.0f, 1.0f);
	//specular = 1.0f - acos(specular) / 1.57079f;
	specular = pow(specular, 4.0f);
	specular *= shadow * color.a;
	//specular *= 2.0f;
	if(specular > 1.0f)
		specular = 1.0f;
	
    //vec4 color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	color.rgb = color.rgb * diffuse + vec3(0.0f, 0.02f, 0.07f) * (1.0f - diffuse);
	color.rgb = color.rgb * (1.0f - fresnel) + vec3(0.65f, 0.85f, 1.0f) * fresnel;
	color.rgb = color.rgb * (1.0f - specular) + vec3(1.0f, 0.93f, 0.8f) * specular;
	finalColor.rgb = color.rgb;
	finalColor.a = 1.0f;
	//finalColor.rgba = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	finalColor.rgb = finalColor.rgb * highlight + vec3(1.0f, 0.0f, 0.0f) * (1.0f - highlight);
	
	float air = exp(-pow(gl_FragCoord.w, 2.0f) / 0.05f) * 0.1f;
	finalColor.rgb = finalColor.rgb * (1.0f - air) + vec3(0.5f, 0.7f, 1.0f) * air;
} 