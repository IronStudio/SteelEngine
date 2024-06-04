#version 460 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in float in_Offset;
layout (location = 3) in float in_Depth;
layout (location = 4) in float in_Opacity;

layout (location = 0) out vec4 out_Color;

layout (binding = 0) uniform UBO {
	float uni_ColorBias;
};


void main() {
	gl_Position = vec4(in_Position + vec2(in_Offset), in_Depth, 1.0);
	out_Color = vec4(uni_ColorBias * in_Color, in_Opacity);
}