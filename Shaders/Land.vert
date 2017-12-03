#version 450

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform mat4 depthMatrix;
layout(location = 2) uniform vec3 lightPos;
layout(location = 3) uniform vec3 cameraPos;
layout(location = 4) uniform vec2 playerPos;
layout(location = 5) uniform mat4 targetDir;
layout(location = 6) uniform float time;
layout(location = 7) uniform float seaLevel;
layout(location = 9) uniform mat3 modelViewMatrix;
layout(location = 12) uniform uint indexCount;
layout(location = 13) uniform uint vertexCount;

layout (location = 2) in vec3 faceTangent;
layout (location = 3) in vec3 faceBitangent;
layout (location = 4) in float desertFactor;

out vec3 pos;
out vec4 shadowCoord;
out float desFactor;
out vec3 light;
out vec3 eye;
out vec3 normal;

struct Vector3 {
	float x, y, z;
};

layout (std430, binding = 0) buffer VERTEX_POSITIONS {
	Vector3 vertexPositions[];
};

layout (std430, binding = 1) buffer INDICES {
	unsigned int indices[];
};

layout (std430, binding = 2) buffer NORMALS {
	Vector3 vertexNormals[];
};

layout (std430, binding = 3) buffer TANGENTS {
	Vector3 vertexTangents[];
};  

vec3 computeNormal() {
	uint baseVertex = gl_VertexID / 3;
	baseVertex *= 3;
	uint triangleInds[3];
	triangleInds[0] = indices[baseVertex];
	triangleInds[1] = indices[baseVertex + 1];
	triangleInds[2] = indices[baseVertex + 2];
	
	vec3 vertices[3];
	for(int i = 0; i < 3; ++i) {
		vertices[i].x = vertexPositions[triangleInds[i]].x;
		vertices[i].y = vertexPositions[triangleInds[i]].y;
		vertices[i].z = vertexPositions[triangleInds[i]].z;
	}
	
	vec3 first = vertices[0] - vertices[1];
	first = normalize(first);
	vec3 second = vertices[0] - vertices[2];
	second = normalize(second);
	vec3 result = cross(first, second);
	if(result.z < 0.0f)
		result *= -1.0f;
	result = normalize(result);
	return result;
}

void main()
{	
	/*gl_Position = projMatrix * vec4(vertPos, 1.0f);
	shadowCoord = depthMatrix * vec4(vertPos, 1.0f);
	mat3 tbn = transpose(mat3(faceTangent, faceBitangent, faceNormal));
	light = tbn * (inverse(mat4(1.0f)) * vec4(lightPos, 0.0f)).xyz;
	eye = tbn * (cameraPos - vertPos);
	pos = vertPos;
	desFactor = desertFactor;*/
	
	/*vec2 position = vertPos.xy + instancePosition.xy;
	//float height = texture(reliefHeightMap, vec2((position.x + 0.5f) / 1025.0f, (position.y + 0.5f) / 513.0f)).r;
	ivec2 flatPos = ivec2(int(position.x), int(position.y));
	float height = interpolate(flatPos, position.x + 0.5f, position.y + 0.5f, 1025.0f, 513.0f, reliefHeightMap);
	height += (texture(reliefDetailMap, vec2((position.x * 8.0f) / 1025.0f, (position.y * 8.0f) / 513.0f)).r - 0.5f) * 0.3f;
	gl_Position = projMatrix * vec4(position.xy, height, 1.0f);
	shadowCoord = depthMatrix * vec4(position.xy, height, 1.0f);
	desFactor = height / 1.0f - 4.1f;
	pos = vec3(position.xy, height);*/
	
	//normal = computeNormal();
	
	uint vertexIndex = indices[gl_VertexID];
	vec3 vertex;
	vertex.x = vertexPositions[vertexIndex].x;
	vertex.y = vertexPositions[vertexIndex].y;
	vertex.z = vertexPositions[vertexIndex].z;
	
	normal.x = vertexNormals[vertexIndex].x;
	normal.y = vertexNormals[vertexIndex].y;
	normal.z = vertexNormals[vertexIndex].z;
	
	vec3 tangent;
	tangent.x = vertexTangents[vertexIndex].x;
	tangent.y = vertexTangents[vertexIndex].y;
	tangent.z = vertexTangents[vertexIndex].z;
	
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = transpose(mat3(tangent, bitangent, normal));
	light = tbn * (inverse(mat4(1.0f)) * vec4(lightPos, 0.0f)).xyz;
	eye = tbn * (cameraPos - vertex);
	
	gl_Position = projMatrix * vec4(vertex.xyz, 1.0f);
	shadowCoord = depthMatrix * vec4(vertex.xyz, 1.0f);
	desFactor = 0.9f;
	pos = vec3(vertex.xyz);
} 