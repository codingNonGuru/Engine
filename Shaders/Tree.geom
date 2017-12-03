#version 430

layout (location = 0) uniform mat4 projMatrix;
layout (location = 1) uniform vec2 offset;
layout (location = 2) uniform vec2 scale;
layout (location = 3) uniform vec3 lightPosition;
//layout (location = 4) uniform vec3 cameraPosition;

layout (points) in;
layout (triangle_strip, max_vertices = 60) out;

in float rot[];
out vec3 fragNormal;
out vec3 fragColor;
//out vec3 tangentSpaceLight;
//out vec3 tangentSpaceEye;

vec2 rotate(vec2 position, inout vec3 normal, float s, float c) {
	normal = vec3(normal.x * c - normal.y * s, normal.x * s + normal.y * c, normal.z);
	
	float x = position.x * c - position.y * s;
	float y = position.x * s + position.y * c;
	
	return vec2(x, y);
}

void emit(vec3 position, float s, float c) {
	vec3 normal = vec3(0.0f, 0.0f, 1.0f);
	vec2 newPos = rotate(position.xy, normal, s, c);
	fragNormal = normal;
	fragColor = vec3(0.0f, 0.0f, 1.0f);
	gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(newPos.x, newPos.y, position.z, 0.0f));
	EmitVertex();
}

void main() {
	//if(gl_in[0].gl_Position.x > offset.x && gl_in[0].gl_Position.x < scale.x && gl_in[0].gl_Position.y > offset.y && gl_in[0].gl_Position.y < scale.y) {
		float s = sin(rot[0]);
		float c = cos(rot[0]);
		
		float scale = 0.01f;
		float height = 0.1f;
		
		emit(vec3(-scale, -scale, height), s, c);
		emit(vec3(-scale, scale, height), s, c);
		emit(vec3(scale, scale, height), s, c);
		EndPrimitive();
		
		emit(vec3(scale, scale, height), s, c);
		emit(vec3(-scale, -scale, height), s, c);
		emit(vec3(scale, -scale, height), s, c);
		EndPrimitive();
		
		for(int i = 0; i < 9; ++i) {
			scale += 0.01f;
			height -= 0.01f;
			
			emit(vec3(-scale, -scale, height), s, c);
			emit(vec3(-scale, scale, height), s, c);
			emit(vec3(scale, scale, height), s, c);
			EndPrimitive();
			
			emit(vec3(scale, scale, height), s, c);
			emit(vec3(-scale, -scale, height), s, c);
			emit(vec3(scale, -scale, height), s, c);
			EndPrimitive();
		}
	//}
}