#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
in vec2 pos;
out vec4 diffuse;
void main(){
diffuse = vec4(.6f, .8f, 0.8f, .6f)*vec4(1, pos, 1);//vec4(.6f,.2f,0.f,.6f)
}