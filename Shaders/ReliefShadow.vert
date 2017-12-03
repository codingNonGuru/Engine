#version 450

layout (location = 0) uniform mat4 viewProjMatrix;

struct Vector3 {
	float x, y, z;
};

layout (std430, binding = 0) buffer VERTEX_POSITIONS {
	Vector3 vertexPositions[];
};

layout (std430, binding = 1) buffer INDICES {
	unsigned int indices[];
}; 

void main()
{
	uint vertexIndex = indices[gl_VertexID];
	vec3 vertex;
	vertex.x = vertexPositions[vertexIndex].x;
	vertex.y = vertexPositions[vertexIndex].y;
	vertex.z = vertexPositions[vertexIndex].z;

	gl_Position = viewProjMatrix * vec4(vertex.xyz, 1.0f);
} 