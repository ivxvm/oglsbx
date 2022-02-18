#version 330 core

out vec3 color;
in vec2 uv;
uniform sampler2D texture_sampler;

void main() {
	color = texture(texture_sampler, uv).rgb;
}
