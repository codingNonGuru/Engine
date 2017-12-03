#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) out vec4 finalColor;
//layout(location = 1) out vec4 specColor;
layout(location = 1) uniform vec3 lightPos;
layout(location = 2) uniform vec3 cameraPos;
layout(location = 4) uniform vec2 firstWaveDisplacement;
layout(location = 5) uniform vec2 secondWaveDisplacement;
layout(location = 6) uniform vec3 skyColor;
layout(location = 7) uniform float firstWeight;
layout(location = 8) uniform float secondWeight;
uniform sampler2D firstWaveMap;
uniform sampler2D secondWaveMap;

in vec3 position;

void main()
{
	vec3 cameraRay = cameraPos - position;
	cameraRay = normalize(cameraRay);
	
	vec2 firstWavePosition = position.xy / 200.0f;
	firstWavePosition += firstWaveDisplacement;
	vec2 secondWavePosition = position.xy / 200.0f;
	secondWavePosition += secondWaveDisplacement;
	vec3 waveNormal_1 = texture(firstWaveMap, firstWavePosition).xyz;
	//waveNormal_1 = waveNormal_1 * 2.0f - 1.0f;
	
	//float factor = (sin(time * 2.0f) + 1.0f) * 0.5f;
	//waveNormal_1 = waveNormal_1 * factor;
	//waveNormal_1 += vec3(0.0f, 0.0f, 1.0f - factor);
	
	vec3 waveNormal_2 = texture(secondWaveMap, secondWavePosition).xyz;
	//waveNormal_2 = waveNormal_2 * 2.0f - 1.0f;
	//waveNormal_2 = waveNormal_2 * (1.0f - factor);
	//waveNormal_2 += vec3(0.0f, 0.0f, factor);
	
	vec3 waveNormal = (waveNormal_1 * firstWeight + waveNormal_2 * secondWeight);
	waveNormal = normalize(waveNormal);
	vec4 reflectAngle = reflect(-vec4(lightPos, 0.0f), vec4(waveNormal, 0.0f));
	vec4 skyReflectAngle = reflect(-vec4(cameraRay, 0.0f), vec4(waveNormal, 0.0f));
	
	vec3 lightColor = vec3(1.0f);//vec3(1.0f, 0.93f, 0.8f);
	float skyHeight = asin(skyReflectAngle.z) / 1.57079f;
	if(skyHeight < 0.0f)
		skyHeight = 0.0f;
	if(skyHeight > 1.0f)
		skyHeight = 1.0f;
	//if(skyHeight > 0.4f && skyHeight < 0.5f)
		//skyHeight = 0.0f;
	//skyHeight = pow(skyHeight, 2.0f);
	vec3 fresnelColor = skyColor.xyz * skyHeight + vec3(1.0f - skyHeight);
	
	float fresnel = dot(cameraRay, waveNormal);
	if(fresnel < 0.0f)
		fresnel = 0.0f;
	fresnel = exp(-pow(fresnel, 2.0f) / 0.3f);
	//fresnel = exp(-fresnel / 0.3f);
	//fresnel = acos(fresnel) / 1.57079f;
	//fresnel = pow(1.0f - fresnel, 2.0f);
	//fresnel = pow(fresnel, pow(2.0f, 2.0f - 4.0f * fresnel));
	//fresnel = pow(fresnel, 0.5f);
	//fresnel = 0.0f;
	//fresnel *= 0.5f;
	//fresnel = 1.0f - fresnel;
	//fresnel = 1.0f - fresnel;
	//fresnel = 0.3f + fresnel * 0.6f;
	
	float specular = dot(reflectAngle, vec4(cameraRay, 0.0f));
	if(specular < 0.0f)
		specular = 0.0f;
	specular = exp(-pow(1.0f - specular, 2.0f) / 0.00002f);
	//specular = 1.0f - acos(specular) / 1.57079f;
	//specular = pow(specular, 2.0f);
	//specular = pow(specular, pow(16.0f, 15.9f - 16.0f * specular));
	//specular = pow(specular, 20.0f);
	//specular = 1.0f - exp(-pow(specular, 4.0f) / 1.5f);
	
	finalColor.a = fresnel + specular * (1.0f - fresnel);
	finalColor.rgb = fresnelColor.rgb * fresnel * (1.0f - specular) + lightColor.rgb * specular;
	finalColor.rgb /= finalColor.a;
	
	//finalColor.rgb = vec3(1.0f, 1.0f, 1.0f);
	//finalColor.a = factor; 
	
	/*vec3 cameraRay = normalize(camera - position);
	vec3 normal = vec3(0.0f, 0.0f, 1.0f);
	float fresnel = dot(cameraRay, normal);
	if(fresnel < 0.0f)
		fresnel = 0.0f;
	fresnel = 1.0f - fresnel;*/
} 