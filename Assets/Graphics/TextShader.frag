#version 330
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable
uniform layout(binding=0) sampler2D glyph;
in vec2 uvs;
uniform vec4 color;
out vec4 rgba;
void main(void){
rgba =vec4(color.rgb,color.a*(texture2D(glyph, uvs).r+.05f));//
}
