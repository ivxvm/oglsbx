#version 330 core

layout(location = 0) in vec3 vertex_pos_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 offset_worldspace;

uniform mat4 vp;
out vec2 uv;

void main(){
	gl_Position = vp * vec4(offset_worldspace + vertex_pos_modelspace,1);
	uv = vertex_uv;
}
