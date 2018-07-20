#version 330

layout(location = 0) out vec4 color_out;

uniform sampler2D sampler_scene_color;
uniform sampler2D sampler_shadow;

void main()
{
	vec3 color = texelFetch(sampler_scene_color, ivec2(gl_FragCoord.xy), 0).rgb; 
	
	bool notBackground =(color[0]!=1.0f); 
	
	if (notBackground)
	{
		float shadowMask = texelFetch(sampler_shadow, ivec2(gl_FragCoord.xy), 0).r; 
		color *= shadowMask;
	}
	
	color_out = vec4(color, 1.0f);
	
}