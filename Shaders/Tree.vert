#version 430

layout (location = 0) uniform mat4 projMatrix;
layout (location = 1) uniform mat4 depthMatrix;
layout (location = 3) uniform vec3 lightPosition;
layout (location = 4) uniform vec3 cameraPosition;

struct Vector {
	float x, y, z;
};

layout (std430, binding = 0) buffer POSITIONS {
	Vector positions[];
};

layout (std430, binding = 1) buffer NORMALS {
	Vector normals[];
};

layout (std430, binding = 2) buffer TANGENTS {
	Vector tangents[];
};

layout (std430, binding = 3) buffer BITANGENTS {
	Vector bitangents[];
};

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTangent;
layout (location = 3) in vec3 vertexBitangent;
layout (location = 4) in vec2 vertexTexCoord;

layout (location = 5) in vec3 instancePosition;
layout (location = 6) in float instanceRotation;
layout (location = 7) in vec3 instanceColor;
layout (location = 8) in uint instanceOffset;

out vec3 position;
out vec3 light;
out vec3 eye;
out vec4 shadowCoord;
out vec2 texCoord;
out vec3 treeColor;

void main()
{ 
	float s = sin(instanceRotation);
	float c = cos(instanceRotation);
	
	uint offset = instanceOffset + gl_VertexID;
	
	//vec3 normal = vec3(vertexNormal.x * c - vertexNormal.y * s, vertexNormal.x * s + vertexNormal.y * c, vertexNormal.z);
	vec3 normal = vec3(normals[offset].x * c - normals[offset].y * s, normals[offset].x * s + normals[offset].y * c, normals[offset].z); 
	texCoord = vertexTexCoord;
	//vec3 tangent = vec3(vertexTangent.x * c - vertexTangent.y * s, vertexTangent.x * s + vertexTangent.y * c, vertexTangent.z);
	//vec3 bitangent = vec3(vertexBitangent.x * c - vertexBitangent.y * s, vertexBitangent.x * s + vertexBitangent.y * c, vertexBitangent.z);
	vec3 tangent = vec3(tangents[offset].x * c - tangents[offset].y * s, tangents[offset].x * s + tangents[offset].y * c, tangents[offset].z);
	vec3 bitangent = vec3(bitangents[offset].x * c - bitangents[offset].y * s, bitangents[offset].x * s + bitangents[offset].y * c, bitangents[offset].z);

	//float x = vertexPosition.x * c - vertexPosition.y * s;
	//float y = vertexPosition.x * s + vertexPosition.y * c;
	//position = vec3(instancePosition.x + x, instancePosition.y + y, instancePosition.z + vertexPosition.z);
	
	float x = positions[offset].x * c - positions[offset].y * s;
	float y = positions[offset].x * s + positions[offset].y * c;
	position = vec3(instancePosition.x + x, instancePosition.y + y, instancePosition.z + positions[offset].z);
	
	mat3 tbn = transpose(mat3(tangent, bitangent, normal));
	light = tbn * (inverse(mat4(1.0f)) * vec4(lightPosition, 0.0f)).xyz;
	eye = tbn * normalize(cameraPosition - position);
	//eye = tbn * (inverse(mat4(1.0f)) * vec4(cameraPosition, 0.0f)).xyz;
	
	treeColor = instanceColor;
	
	//if(dot(cameraPosition - position, normal) < 0.0f)
		//normal *= -1.0f; 
	
    gl_Position = projMatrix * vec4(position, 1.0f);
    shadowCoord = depthMatrix * vec4(position, 1.0f); 
}