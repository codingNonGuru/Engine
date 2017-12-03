#version 430

layout (location = 3) uniform vec3 lightPosition;
layout (location = 4) uniform vec3 cameraPosition;

layout (location = 0) out vec4 finalColor;

uniform sampler2D shadowMap;
uniform sampler2D foliageSpecular;
uniform sampler2D foliageDiffuse;
uniform sampler2D foliageMixFactor;
uniform sampler2D steppeNormalMap;

in vec3 position;
in vec3 light;
in vec3 eye;
in vec4 shadowCoord;
in vec2 texCoord;
in vec3 treeColor;

//in vec3 fragNormal;
//in vec3 fragColor;
//in vec3 tangentSpaceLight;
//in vec3 tangentSpaceEye;

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
    float bias = 0.01f;
	
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

void main() {
	vec3 finalNormal = texture(steppeNormalMap, texCoord * 0.04f).rgb;
	finalNormal = finalNormal * 2.0f - 1.0f;
	finalNormal = normalize(finalNormal);
	//vec3(0.0f, 0.0f, 1.0f);//normalize(normal);

	vec4 color = texture(foliageDiffuse, texCoord).rgba;
	float texSpecular = texture(foliageSpecular, texCoord).r;
	float mixFactor = texture(foliageMixFactor, texCoord).r;

	vec3 cameraRay = normalize(cameraPosition - position);

	float shadow = computeShadow(shadowCoord);

	vec3 finalLight = normalize(light);

	float diffuse = dot(finalNormal, finalLight);
	//diffuse = clamp(diffuse, 0.0f, 1.0f);
	if(diffuse < 0.0f) 
		diffuse = 0.0f;
	diffuse *= shadow * 0.8f + 0.2f;
	diffuse = diffuse * 0.8f + 0.2f;
	
	vec4 refAngle = reflect(-vec4(finalLight, 0.0f), vec4(finalNormal, 0.0f));
	float dot = dot(refAngle, vec4(normalize(eye), 0.0f));
	//float specular = clamp(dot(reflect(light, finalNormal), eye), 0.0f, 1.0f);
	float specular = clamp(dot, 0.0f, 1.0f);
	//specular = 1.0f - acos(specular) / 1.57079f;
	specular = pow(specular, 4.0f);
	specular *= shadow;// * texSpecular;
	specular *= 0.2f;
	//if(specular > 1.0f)
		//specular = 1.0f;
	
	//finalColor.rgb = vec3(0.05f, 0.2f, 0.0f);//color.rgb;
	finalColor.rgb = treeColor.rgb;
	//finalColor.rgb = color.rgb * mixFactor + treeColor.rgb * (1.0f - mixFactor);
	finalColor.rgb = finalColor.rgb * diffuse + vec3(0.0f, 0.02f, 0.07f) * (1.0f - diffuse);
	//finalColor.rgb = finalColor.rgb * (1.0f - specular) + vec3(1.0f, 0.93f, 0.8f) * specular;
	finalColor.rgb = finalColor.rgb * (1.0f - specular) + vec3(1.0f) * specular;
	finalColor.a = 1.0f;
	
	float air = exp(-pow(gl_FragCoord.w, 2.0f) / 0.05f) * 0.1f;
	finalColor.rgb = finalColor.rgb * (1.0f - air) + vec3(0.5f, 0.7f, 1.0f) * air;
	
	//if(color.a < 0.07f)
		//discard;
	//if(color.a > 0.1f)
		//finalColor.rgb = vec3(1.0f, 0.0f, 0.0f);
}