#version 330

layout(location = 0) out vec4 color_out;

uniform sampler2D texture_sampler1;
uniform sampler2D texture_sampler2;

void main()
{
	vec3 color = texelFetch(texture_sampler1, ivec2(gl_FragCoord.xy), 0).rgb; 
	float intensity = texelFetch(texture_sampler2, ivec2(gl_FragCoord.xy), 0).r; 
	color_out = vec4(color*intensity, 1.0f);
}