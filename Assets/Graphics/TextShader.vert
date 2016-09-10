#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
layout(location=0)in vec4 coord;
out vec2 uvs;
 
void main(void){
  vec2 p=2.f*(coord.xy)-1.f;
  gl_Position = vec4(p, -1.f, 1.f);
  uvs=coord.zw;
}