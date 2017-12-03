#version 330
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 viewProjMatrix;
layout(location = 10) uniform vec2 stencilPosition;
layout(location = 11) uniform vec2 stencilScale;

layout (location = 0) in vec3 vertPos;

layout (location = 5) in vec3 objectPos;
layout (location = 6) in float scale;
layout (location = 7) in float rotation;

uniform sampler2D pavementStencil;

void main()
{	
	float s = sin(rotation);
	float c = cos(rotation);
	
	float x = vertPos.x * c - vertPos.y * s;
	float y = vertPos.x * s + vertPos.y * c;
	
	vec2 stencilCoord = objectPos.xy - stencilPosition.xy;
	stencilCoord.x /= stencilScale.x;
	stencilCoord.y /= stencilScale.y;
	float heightOffset = texture(pavementStencil, stencilCoord).r * 0.05f;
	
	vec3 realPos = objectPos + vec3(x, y, vertPos.z * scale + heightOffset);
	
	gl_Position = viewProjMatrix * vec4(realPos, 1.0f);
} 