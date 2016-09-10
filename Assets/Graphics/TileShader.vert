#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
uniform ivec2 screensize;
in vec2 position;
out vec2 pos;
void main() {
	vec2 p = 2.f * (position / screensize) - 1.f;
	gl_Position = vec4(p, 0.f, 1.f);
	pos = position / screensize;
}