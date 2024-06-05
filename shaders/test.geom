#version 460 core

#define VERTEX_COUNT 14

layout (points) in;
layout (location = 0) in vec4 in_Color[];

layout (triangle_strip, max_vertices = VERTEX_COUNT) out;
layout (location = 0) out vec4 out_Color;

layout (std140, binding = 0) uniform UBO {
	float uni_ColorBias;
	mat4 uni_Camera;
};


void main() {
	vec3 vertices[VERTEX_COUNT] = {
		vec3(0.5, 0.5, -0.5),
		vec3(-0.5, 0.5, -0.5),
		vec3(0.5, -0.5, -0.5),
		vec3(-0.5, -0.5, -0.5),
		vec3(-0.5, -0.5, 0.5),
		vec3(-0.5, 0.5, -0.5),
		vec3(-0.5, 0.5, 0.5),
		vec3(0.5, 0.5, -0.5),
		vec3(0.5, 0.5, 0.5),
		vec3(0.5, -0.5, -0.5),
		vec3(0.5, -0.5, 0.5),
		vec3(-0.5, -0.5, 0.5),
		vec3(0.5, 0.5, 0.5),
		vec3(-0.5, 0.5, 0.5),
	};

	out_Color = in_Color[0];

	for (int i = 0; i < VERTEX_COUNT; ++i) {
		gl_Position = gl_in[0].gl_Position + uni_Camera * vec4(vertices[i], 1.0);
		EmitVertex();
	}

	EndPrimitive();
}