#version 450

layout(location = 1) uniform mat4 depthMatrix;
layout(location = 2) uniform vec3 lightPos;
layout(location = 3) uniform vec3 cameraPos;
layout(location = 7) uniform float seaLevel;
layout(location = 8) uniform vec2 specialTile;
layout(location = 10) uniform vec2 stencilPosition;
layout(location = 11) uniform vec2 stencilScale;

uniform sampler2D steppeDiffuseMap;
uniform sampler2D steppeNormalMap;
uniform sampler2D desertDiffuseMap;
uniform sampler2D desertNormalMap;
uniform sampler2D shadowMap;
uniform sampler2D pavementStencil;
uniform sampler2D pavementDiffuse;
uniform sampler2D pavementNormal;
uniform sampler2D roadStencil;
uniform sampler2D provinceDiffuse;
uniform sampler2D borderStencil;

in vec3 pos;
in vec4 shadowCoord;
in float desFactor;
in vec3 light;
in vec3 eye;
in vec3 normal;

layout (location = 0) out vec4 finalColor;

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
    float bias = 0.00001f;
	
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			float intensity = currentDepth - closestDepth;
			if(intensity < 0.0f)
				intensity = 0.0f; 
			shadow += currentDepth - bias > pcfDepth ? 0.0f : 1.0f;        
		}    
	shadow /= 9.0f;

    return shadow;
}

float atan2(float y, float x) {
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

void main() {
	vec3 vertNorm;// = normalize(normal);
	vec2 texCoord = pos.xy / 120.0f;
	vec3 position = vec3(pos.xyz);
	vec2 stencilCoord = position.xy - stencilPosition.xy;
	stencilCoord.x /= stencilScale.x;
	stencilCoord.y /= stencilScale.y;
	vec3 politicalColor = texture(provinceDiffuse, vec2(pos.x / 1024.0f, pos.y / 512.0f)).rgb;
	vec4 steppeColor = texture(steppeDiffuseMap, texCoord).rgba;
	vec4 desertColor = texture(desertDiffuseMap, texCoord).rgba;
	float paveStencilAlpha = texture(pavementStencil, stencilCoord).r;
	float roadStencilAlpha = texture(roadStencil, stencilCoord).r;
	vec4 pavement = texture(pavementDiffuse, pos.xy * 3.0f).rgba;
	float compoundStencilAlpha = roadStencilAlpha + paveStencilAlpha;
	if(compoundStencilAlpha > 1.0f)
		compoundStencilAlpha = 1.0f;
	if(compoundStencilAlpha > paveStencilAlpha)
		paveStencilAlpha = compoundStencilAlpha;
	if(paveStencilAlpha > pavement.a)
		paveStencilAlpha = 1.0f;
	else 
		paveStencilAlpha = 0.0f;
	vec3 steppeNormal = texture(steppeNormalMap, texCoord).rgb;
	steppeNormal = steppeNormal * 2.0f - 1.0f;
	vec4 paveNormal = texture(pavementNormal, pos.xy * 3.0f).rgba;
	paveNormal.rgb = paveNormal.rgb * 2.0f - 1.0f;
	vertNorm = paveNormal.rgb * paveStencilAlpha + steppeNormal * (1.0f - paveStencilAlpha);
	//vertNorm = paveNormal.rgb * (paveStencilAlpha + 0.1f) + steppeNormal * (0.9f + 1.0f - paveStencilAlpha);
	//vertNorm = vec3(0.0f, 0.0f, 1.0f);
	//vertNorm = normalize(vertNorm);
	//vertNorm = normalize(normal);
	vec4 border = texture(borderStencil, stencilCoord).rgba;
	
	float factor = 0.0f; 
	if(position.z > seaLevel + 0.2f)
		factor = 1.0f;
	else
		factor = exp(-pow(position.z - (seaLevel + 0.2f), 2.0f) / 0.00001f);
	//factor *= desFactor;
	
	vec4 Light = vec4(normalize(light), 0.0f);

	vec3 cameraRay = normalize(eye);// - position;
	//vec3 cameraRay = cameraPos - position;
	//cameraRay = normalize(cameraRay);
	
	float shadow = computeShadow(shadowCoord);
	
	float fresnel = dot(cameraRay, vertNorm);
	if(fresnel < 0.0f)
		fresnel = 0.0f;
	fresnel = 1.0f - fresnel;
	fresnel = pow(fresnel, pow(2.0f, 7.99f - 8.0f * fresnel));
	//fresnel *= paveNormal.a * paveStencilAlpha + 1.0f * (1.0f - paveStencilAlpha);
	fresnel *= 0.7f;
	//fresnel = exp(-fresnel / 0.2f);
	//fresnel *= shadow;
	
	//float diffuse = dot(normalize(light), vertNorm);
	float diffuse = dot(normalize(light), vertNorm);
	if(diffuse < 0.0f)
		diffuse = 0.0f;
	diffuse *= shadow * 0.8f + 0.2f;
	diffuse = diffuse * 0.8f + 0.2f;
	
	vec4 refAngle = reflect(-Light, vec4(vertNorm, 0.0f));
	float dot = dot(refAngle, vec4(cameraRay, 0.0f));
	float specular = clamp(dot, 0.0f, 1.0f);
	//specular = 1.0f - acos(specular) / 1.57079f;
	//float correctionFactor = 200.0f * paveStencilAlpha + 200.0f * (1.0f - paveStencilAlpha);
	//specular = pow(specular, correctionFactor);
	specular = exp(-pow(1.0f - specular, 2.0f) / 0.001f);
	specular *= shadow;
	specular *= paveNormal.a * paveStencilAlpha + 1.0f * (1.0f - paveStencilAlpha);
	specular *= 1.0f - border.a * 0.7f;
	//specular *= 2.0f;
	if(specular > 1.0f)
		specular = 1.0f;

	//finalColor.rgba = vec4(0.92f, 0.92f, 0.89f, 1.0f);
	//finalColor.rgb = finalColor.rgb * factor + vec3(1.0f, 0.85f, 0.65f) * (1.0f - factor);
	float steppeAlpha = factor;
	/*if(steppeColor.a > factor)
		steppeAlpha = 0.0f;
	else
		steppeAlpha = 1.0f;*/
	finalColor.rgb = steppeColor.rgb * steppeAlpha + desertColor.rgb * (1.0f - steppeAlpha);
	//finalColor.rgb = pavement.rgb * paveStencilAlpha + (1.0f - paveStencilAlpha) * finalColor.rgb;
	finalColor.a = 1.0f;
	//finalColor.rgb = vec3(1.0f, 0.85f, 0.65f) * (1.0f - factor) + vec3(0.1f, 0.3f, 0.03f) * factor;
	finalColor.rgb = pavement.rgb * paveStencilAlpha + (1.0f - paveStencilAlpha) * finalColor.rgb;
	finalColor.rgb = finalColor.rgb * (1.0f - border.a) + border.rgb * border.a;  
	finalColor.rgb = finalColor.rgb * diffuse + vec3(0.0f, 0.02f, 0.07f) * (1.0f - diffuse);
	//finalColor.rgb = finalColor.rgb * (1.0f - fresnel) + vec3(0.75f, 0.9f, 1.0f) * fresnel;
	finalColor.rgb = finalColor.rgb * (1.0f - specular) + vec3(specular);//vec3(1.0f, 0.93f, 0.8f) * specular;
	//finalColor.rgb = politicalColor.rgb;
		
	//finalColor.rgb *= shadow;
	//finalColor.rgb = vec3(desFactor, 1.0f - desFactor, 0.0f);
		
	if(position.z < seaLevel) {
		vec3 cameraRay = cameraPos - position;
		cameraRay = normalize(cameraRay);
		float heightDifference = seaLevel - position.z;
		float t = heightDifference / cameraRay.z;
		vec3 waterDepth =  vec3(t * cameraRay.x, t * cameraRay.y, seaLevel - position.z);
		
		float cameraFactor = 1.0f - exp(-length(waterDepth) / 6.25f);
		if(cameraFactor > 1.0f)
			cameraFactor = 1.0f;
		
		heightDifference = exp(-heightDifference / 7.8f);
		vec3 shallowOceanColor = vec3(0.0f, 0.95f, 0.8f);
		vec3 deepOceanColor = vec3(0.0f, 0.03f, 0.02f);
		vec3 oceanColor = shallowOceanColor * heightDifference + deepOceanColor * (1.0f - heightDifference);
		vec3 waveColor = vec3(1.0f, 1.0f, 1.0f);
		
		finalColor.rgb = finalColor.xyz * (1.0f - cameraFactor);
		finalColor.rgb += oceanColor.xyz * cameraFactor;
	} else {
		//float air = exp(-pow(gl_FragCoord.w, 2.0f) / 0.05f) * 0.1f;
		//finalColor.rgb = finalColor.rgb * (1.0f - air) + vec3(0.5f, 0.7f, 1.0f) * air;
	}
	
	/*float count = pos.x / 1.0f;
	if(pos.x - float(int(count)) * 1.0f < 0.5f)
		finalColor.rgb = vec3(1.0f, 0.0f, 0.0f);
	else
		finalColor.rgb = vec3(1.0f, 1.0f, 1.0f);*/
	
	//finalColor.rgb = normal;
	//finalColor.rgb = position.z - float(int(position.z)) < 0.5f ? vec3(1.0f, 0.5f, 0.0f) : vec3(0.0f, 0.0f, 1.0f);
}