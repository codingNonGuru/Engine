#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projMatrix;
layout(location = 1) uniform vec2 scale;
layout(location = 2) uniform vec2 displacement;
layout(location = 3) uniform float depth;
layout(location = 4) uniform float opacity;
//layout(location = 1) uniform vec3 lightPos;
//layout(location = 2) uniform vec3 cameraPos;

layout (location = 0) in vec3 vertPos;
//layout (location = 1) in vec3 vertCol;

out vec2 texCoord;
out vec2 position;

void main() {
	texCoord = vertPos.xy;
	gl_Position = projMatrix * vec4(vertPos.x * scale.x + displacement.x, vertPos.y * scale.y + displacement.y, depth, 1.0f);
} 