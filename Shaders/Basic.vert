#version 450

layout (location = 0) uniform mat4 projMatrix;

layout (location = 0) in vec2 vertPos;

void main() {
	float factor = 2.5f;
	/*if(gl_VertexID % 16 == 0)
		factor = 2.5f;
	else
		factor = 0.0f;*/
	gl_Position = projMatrix * vec4(vertPos.x * factor, vertPos.y * factor, 0.0f, 1.0f);
} 