#version 450

layout(location = 3) uniform uint mode;

layout (binding = 0) uniform SCREEN_PIXELS {
	float weights[61];
};

uniform sampler2D screenPixels;
uniform sampler2D lastCapture;

layout (location = 0) out vec4 finalColor;

void main() {
	ivec2 coords = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	if(mode == 2) {
		vec4 first = texelFetch(screenPixels, coords, 0).rgba;
		vec4 last = texelFetch(lastCapture, coords, 0).rgba;
		float alpha = first.r;
		//alpha *= alpha;
		//alpha = 0.0f;
		//alpha *= 1.5f;
		//alpha = 0.0f;
		//alpha = exp(-pow(1.0f - alpha, 2.0f) / 0.4f);
		alpha = pow(alpha, 0.9f);
		finalColor.rgb = last.rgb * (1.0f - alpha) + vec3(1.0f) * alpha;
		finalColor.a = 1.0f; 
	}
	else {
		vec4 color = vec4(0.0f);
		vec4 sum = vec4(0.0f);
		if(mode == 0) {
			for(int x = 0; x < 61; ++x) {
				ivec2 newCoords = coords + ivec2(x - 30, 0);
				//color = texelFetch(screenPixels, coords + ivec2(x - 30, 0), 0).rgba;
				color = texture(screenPixels, vec2(float(newCoords.x) / 2560.0f, float(newCoords.y) / 1440.0f)).rgba;
				float intensity = (color.r + color.g + color.b) * 0.3333f;
				/*if(intensity > 0.9f)
					intensity = 1.0f;
				else 
					intensity = 0.0f;*/
				intensity = exp(-pow(1.0f - intensity, 2.0f) / 0.03f);
				intensity = pow(intensity, pow(4.0f, 2.0f - 4.0f * intensity));
				//intensity *= intensity;			
				sum.r += intensity * weights[x];
			}
		}
		else {
			for(int y = 0; y < 61; ++y) {
				ivec2 newCoords = coords + ivec2(0, y - 30);
				color = texture(screenPixels, vec2(float(newCoords.x) / 2560.0f, float(newCoords.y) / 1440.0f)).rgba;	
				sum.r += color.r * weights[y] * 1.2f;
			}
		}
		
		finalColor.rgb = sum.rgb;
		finalColor.a = 1.0f;
	}
} 