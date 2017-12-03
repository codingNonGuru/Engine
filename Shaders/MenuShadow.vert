#version 450
//#extension GL_ARB_explicit_uniform_location : enable

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
	texCoord.xy = vertPos.xy;
	position.x = vertPos.x * scale.x + displacement.x;
	position.y = vertPos.y * scale.y + displacement.y;
	gl_Position = projMatrix * vec4(position.x, position.y, depth, 1.0f);
	position.x /= 2560.0f;
	position.y /= 1440.0f;
	position.y = 1.0f - position.y;
}
