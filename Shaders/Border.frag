#version 450

uniform sampler2D borderAlpha;
uniform sampler2D borderContour;
uniform sampler2D borderChecker;

in float fragScale;
in vec2 texCoord;

layout (location = 0) out vec4 finalColor;

void main() {
	float alpha = texture(borderAlpha, texCoord).r;
	float checker = texture(borderChecker, texCoord).r;
	float baseColor = alpha;
	float contour = texture(borderContour, texCoord).r;
	if(alpha < contour)
		alpha = contour;
	vec3 base = vec3(0.8f, 0.0f, 0.0f) * checker + vec3(1.0f, 0.8f, 0.0f) * (1.0f - checker);
	base = base.rgb * baseColor + vec3(0.0f) * (1.0f - baseColor);
	vec3 final = vec3(0.0f) * contour + base.rgb * (1.0f - contour);
    finalColor = vec4(final.rgb, alpha);
} 