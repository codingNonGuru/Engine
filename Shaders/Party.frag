#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) uniform mat4 projMatrix;
layout (location = 2) uniform vec3 lightPos;
layout (location = 3) uniform vec3 cameraPos;

layout (location = 0) out vec4 finalColor;

uniform sampler2D shadowMap;

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

in vec3 position;
in vec3 normal;
in vec4 shadowCoord;

void main()
{
	vec3 cameraRay = cameraPos - position;
	cameraRay = normalize(cameraRay);
	vec3 norm = normalize(normal);
	
	float shadow = computeShadow(shadowCoord);
	
	float diffuse = dot(lightPos, norm);
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
	//diffuse = diffuse * 0.9f + 0.1f;
	
	float specular = dot(reflect(-lightPos, norm), cameraRay);
	specular = specular < 0.0f ? 0.0f : specular;
	specular = exp(-pow(1.0f - specular, 2.0f) / 0.05f);
	specular *= shadow;
	//specular *= 0.3f;
	
	finalColor.rgb = vec3(1.0f, 0.0f, 0.0f) * diffuse + vec3(0.0f, 0.02f, 0.07f) * (1.0f - diffuse);
	finalColor.rgb = finalColor.rgb * (1.0f - specular) + vec3(1.0f, 0.93f, 0.8f) * specular;
	//finalColor.rgb = objCol.rgb;// + specular;
	finalColor.a = 1.0f;
} 