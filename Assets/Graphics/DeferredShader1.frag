#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shader_storage_buffer_object : enable
in Vdata { 
vec3 positions, normals, tangents;
vec2 uvs;
};
uniform layout (binding = 0) sampler2D AmbientTexture;
uniform layout (binding = 1) sampler2D DiffuseTexture;
uniform layout (binding = 2) sampler2D SpecularTexture;

uniform layout(location = 3) uint id;
uniform layout(location = 4) ivec2 cpos;
uniform layout(location = 6) vec2 tlevel;

layout (location = 0) out vec4 diffuse;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 tangent;
layout (location = 3) out vec4 specular;
layout (location = 4) out vec4 ambient;
layout (location = 5) out vec4 position;
layout (binding = 1, std430) writeonly buffer cursor{
	uint lastid;
	};

//keyframe interpolation
//
//
void main() {
	diffuse = texture(DiffuseTexture, uvs);
	if (diffuse.a < tlevel.x || tlevel.y < diffuse.a) discard;
	if (ivec2(gl_FragCoord) - cpos == ivec2(0))lastid = id;
	specular = texture(SpecularTexture, uvs);
	ambient = texture(AmbientTexture, uvs);
	normal = vec4(normals, 1.f);
	tangent = vec4(tangents, 1.f);
	position = vec4(positions, 1.f);
	}
