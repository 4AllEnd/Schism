#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable
in vec2 uvs;
uniform layout (binding = 0) sampler2D glyph;
out vec4 rgba;

void main(void){
	rgba = texture2D(glyph, uvs);
	}
