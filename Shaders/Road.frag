#version 450

uniform sampler2D roadAlpha;

in float fragScale;
in vec2 texCoord;

layout (location = 0) out float finalColor;

void main() {
    finalColor = texture(roadAlpha, texCoord).r;
} 