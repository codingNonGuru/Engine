#version 450

layout (location = 1) uniform vec2 stencilOffset;
layout (location = 2) uniform vec2 stencilScale;

layout (location = 0) in vec2 position;
layout (location = 1) in float instanceScale;
layout (location = 2) in float instanceRotation;

out vec2 worldPos;
out float scale;
out float rotation;

void main()
{
	scale = instanceScale;
	rotation = instanceRotation;
	worldPos = position.xy;
    gl_Position = vec4(position.x - stencilOffset.x, stencilScale.y - (position.y - stencilOffset.y), 0.5f, 1.0f); 
}