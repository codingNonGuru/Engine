#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 1) uniform vec3 cameraPos;
layout(location = 2) uniform vec3 bgroundCol;
layout(location = 3) uniform float seaLevel;
//uniform sampler2D heightMap;

in vec3 pos;

layout (location = 0) out vec4 finalColor;

void main() {
	vec3 cameraRay = cameraPos - pos;
	cameraRay = normalize(cameraRay);
	
	finalColor.rgb = bgroundCol.xyz;
	finalColor.a = 1.0f;
	
	//if(pos.z < seaLevel) {
		float heightDifference = seaLevel - pos.z;
		float t = heightDifference / cameraRay.z;
		vec3 waterDepth = vec3(t * cameraRay.x, t * cameraRay.y, seaLevel - pos.z);
		
		heightDifference = exp(-pow(heightDifference, 1.6f) / 4.0f);
		vec3 shallowOceanColor = vec3(0.0f, 1.0f, 0.85f);
		vec3 deepOceanColor = vec3(0.0f, 0.1f, 0.2f);
		vec3 oceanColor = shallowOceanColor * heightDifference + deepOceanColor * (1.0f - heightDifference);
		
		float Fac = 0.2f + (length(waterDepth) * 0.15f);
		if(Fac > 1.0f)
			Fac = 1.0f;
		//finalColor.rgb = finalColor.rgb * (1.0f - Fac);
		finalColor.rgb = oceanColor.rgb;
		finalColor.a = Fac;
	//}
}