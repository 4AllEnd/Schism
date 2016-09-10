#version 330
#extension GL_ARB_shading_language_420pack: enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shader_storage_buffer_object : enable
in vec2 uvs;
uniform layout (location = 3) uint id;
uniform layout (location = 4) ivec2 cpos;
uniform layout (binding = 0) sampler2D glyph;
out vec4 rgba;
layout (binding = 1, std430) writeonly buffer cursor {
	uint lastid;
	};
void main(void){
	if (ivec2(gl_FragCoord) - cpos == ivec2(0))lastid = id;
	//lastid = uint(gl_FragCoord.x);//id;
	rgba = texture2D(glyph, uvs);
	}
