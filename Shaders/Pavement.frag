#version 450

in vec2 pos;
in float fragScale;

layout (location = 0) out float finalColor;

void main() {
 	float alpha = length(pos);
    finalColor = alpha < fragScale ? 1.0f : exp(-pow(alpha - fragScale, 2.0f) / 0.07f);   
} 