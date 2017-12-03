#version 450

layout (location = 0) uniform mat4 finalMatrix;
layout (location = 1) uniform ivec2 worldSize;

layout (location = 0) in vec3 vertexPosition;

//layout (location = 5) in float temperature;

layout (location = 5) in vec2 instancePosition;
layout (location = 6) in float velocity;

out float color;

void main() {
	color = velocity * 0.5f;
	//color = velocity - 1.8f;
	//color *= 0.8f;
	//color *= 0.6f;
	/*if(color < 0.0f)
		color = 0.0f;
	else
		color *= 1.5f;*/

	//color = length(velocity) * 40.0f;
	/*if(color > 1.0f)
		color = 1.0f;
	else
		color = 0.0f;*/

	int id = gl_InstanceID;
	float x = float(id % worldSize.x);
	float y = float(id / worldSize.x);

	vec3 position;
	//position.xy = instancePosition.xy * 2.5f + vec2(5.0f, 5.0f);
	position.xy = vec2(x, y) * 2.0f + vec2(5.0f, 5.0f);
	position.z = 0.0f;
	gl_Position = finalMatrix * vec4(vertexPosition + position, 1.0f);
}
