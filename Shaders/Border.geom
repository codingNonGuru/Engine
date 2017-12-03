#version 450

layout(location = 0) uniform mat4 projMatrix;
layout (location = 1) uniform vec2 stencilOffset;
layout (location = 2) uniform vec2 stencilScale;

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec2 worldPos[];
in float scale[];
in float rotation[];
out float fragScale;
out vec2 texCoord;

void emit(float x, float y, float fixScale, float s, float c) {
	fragScale = scale[0];
	texCoord = vec2(x * 0.5f + 0.5f, y * 0.5f + 0.5f);
	texCoord.x /= 8.0f;
	texCoord.x *= scale[0] * 8.0f;
	texCoord.y /= 8.0f;
	x *= scale[0] * 0.5f;
	y *= fixScale;    
	gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(x * c - y * s, x * s + y * c, 0.0f, 0.0f)); 
	EmitVertex();
}

void main() {
	float fixScale = 0.08f;
	float s = sin(rotation[0]);
	float c = cos(rotation[0]);
	bool check = false;
	for(float i = -0.5f; i <= 0.5f; i += 0.1f) {
		float adjSin = sin(3.1415f - rotation[0]);
		float adjCos = cos(3.1415f - rotation[0]);
		//float adjSin = sin(rotation[0]);
		//float adjCos = cos(rotation[0]);
		vec2 end = vec2(i, 0.0f);
		end = worldPos[0] + vec2(end.x * adjCos - end.y * adjSin, end.x * adjSin + end.y * adjCos) * scale[0];
		if(end.x > stencilOffset.x && end.y > stencilOffset.y && end.x < stencilOffset.x + stencilScale.x && end.y < stencilOffset.y + stencilScale.y)
			check = true;
	}  
	if(check == true) {
		emit(-1.0f, -1.0f, fixScale, s, c);
		emit(-1.0f, 1.0f, fixScale, s, c);
		emit(1.0f, 1.0f, fixScale, s, c);
	    
	    EndPrimitive();
	    
	    emit(1.0f, 1.0f, fixScale, s, c);
		emit(-1.0f, -1.0f, fixScale, s, c);
		emit(1.0f, -1.0f, fixScale, s, c);
	    
	    EndPrimitive();
	}
}