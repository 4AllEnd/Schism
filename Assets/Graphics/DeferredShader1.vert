#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
layout (location = 0) in vec3  position;
layout (location = 1) in vec2  UV;
layout (location = 2) in vec3  normal;
layout (location = 3) in vec3  tangent;
uniform layout(location = 0) mat4 projview;
uniform layout(location = 1) mat3 model;
uniform layout(location = 2) vec3 wposition;
out Vdata {
vec3 positions, normals, tangents;
vec2 uvs;
};
void main(){	
	positions = model * position + wposition;
	gl_Position = projview * vec4(positions, 1.f);
	normals = model * normal;
	tangents = model * tangent;
	uvs = UV;
}
