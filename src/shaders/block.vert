#version 330 core

layout(location = 0) in vec3 vertex_pos_modelspace;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 mvp;
out vec2 uv;

void main(){
	gl_Position = mvp * vec4(vertex_pos_modelspace,1);
	uv = vertex_uv;
}
