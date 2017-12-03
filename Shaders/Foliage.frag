#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;

layout(location = 0) out vec4 finalColor;

uniform sampler2D shadowMap;
uniform sampler2D foliageAlpha;

in vec3 position;
in vec3 faceNormal;
in vec3 color;
in vec4 shadowCoord;
in vec2 texCoord;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = 0.005f;
	
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x) {
		for(int y = -2; y <= 2; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 0.5f : 1.0f;        
		}    
	}
	shadow /= 25.0f;

    return shadow;
} 

void main()
{
	vec3 cameraRay = cameraPos - position;
	cameraRay = normalize(cameraRay);
	vec3 faceNorm = normalize(faceNormal);
	
	float specular = clamp(dot(reflect(-lightPos, faceNorm), cameraRay), 0.0f, 1.0f);
	specular = acos(specular) / 1.57079f;
	specular = 1.0f - specular;
	specular *= 0.5f;
	
	float diffuse = clamp(dot(lightPos, faceNorm), 0.0f, 1.0f);
	diffuse = acos(diffuse) / 1.57079f;
	float diffuseAngle = diffuse;
	diffuse = 1.0f - diffuse;
	if(diffuse < 0.0f)
		diffuse = 0.0f;
	
	diffuse = diffuse * 0.4f + 0.6f;
	float shadow = ShadowCalculation(shadowCoord);
	if(shadow < 1.0f) {
		specular = 0.0f;
		float shadowDiffuse = 0.3f;// + diffuseAngle * 0.4f;
		if(shadowDiffuse < diffuse)
			diffuse = shadowDiffuse;
	}
	
	vec4 leafColor = texture(foliageAlpha, texCoord).rgba;
    finalColor.rgb = leafColor.rgb;
	finalColor.rgb *= diffuse;
	finalColor.rgb = finalColor.rgb * (1.0f - specular) + vec3(specular);
	finalColor.a = 1.0f;
	if(leafColor.a < 0.5f)
		discard;
	//finalColor.a = 1.0f;
	
	/*float Fac = 1.8f - gl_FragCoord.z;
	vec4 airColor = vec4(0.258, 0.656, 0.97, 1.0);
	finalColor = color * Fac; 
	finalColor += airColor * (1 - Fac);*/
} 