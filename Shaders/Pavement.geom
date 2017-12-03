#version 450

layout(location = 0) uniform mat4 projMatrix;
layout (location = 1) uniform vec2 stencilOffset;
layout (location = 2) uniform vec2 stencilScale;

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec2 worldPos[];
in float scale[];
out vec2 pos;
out float fragScale;

void main() {
	float fixScale = 1.0f;
	if(worldPos[0].x > stencilOffset.x && worldPos[0].x < stencilOffset.x + stencilScale.x && worldPos[0].y > stencilOffset.y && worldPos[0].y < stencilOffset.y + stencilScale.y) {
		pos = vec2(-fixScale, -fixScale);
		fragScale = scale[0];    
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, -fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    pos = vec2(-fixScale, fixScale);
	    fragScale = scale[0];
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    pos = vec2(fixScale, fixScale);
	    fragScale = scale[0];
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    EndPrimitive();
	    
	    pos = vec2(fixScale, fixScale);
	    fragScale = scale[0];
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    pos = vec2(-fixScale, -fixScale);
	    fragScale = scale[0];
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, -fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    pos = vec2(fixScale, -fixScale);
	    fragScale = scale[0];
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, -fixScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    
	    EndPrimitive();
	}
}