#version 460 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec4 in_Color;

layout (location = 0) out vec4 out_Color;

layout (std140, binding = 0) uniform UBO {
	float uni_ColorBias;
	mat4 uni_Camera;
};


void main() {
	gl_Position = uni_Camera * vec4(in_Position, 1.0);
	out_Color = uni_ColorBias * in_Color;
	gl_PointSize = 1;
}