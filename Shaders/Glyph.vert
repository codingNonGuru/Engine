#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;

layout (location = 0) in vec2 vertPos;

layout (location = 2) in vec2 objectPosition;
layout (location = 3) in vec2 objectScale;
layout (location = 4) in vec4 objectColor;
layout (location = 5) in vec2 texScale;
layout (location = 6) in vec2 texOffset;

out vec2 texCoord;
out vec4 color;

void main() {
	color = objectColor;
	
	texCoord = texOffset + vec2(vertPos.x * texScale.x, vertPos.y * texScale.y);
	
	vec2 position = vec2(vertPos.x * objectScale.x, vertPos.y * objectScale.y) + objectPosition;
	gl_Position = projMatrix * vec4(position.x, position.y, 0.5f, 1.0f);
} 