#version 330

layout(location = 0) out vec4 color_out;

uniform sampler2D sampler_color;
uniform sampler2D sampler_lighting;
uniform bool enable_border; 
uniform sampler2D sampler_border;

void main()
{
	vec3 color = texelFetch(sampler_color, ivec2(gl_FragCoord.xy), 0).rgb; 
	
	bool notBackground =(color[0]!=1.0f); 
	
	if (notBackground)
	{
		color *= texelFetch(sampler_lighting, ivec2(gl_FragCoord.xy), 0).r;
		
		if (enable_border) 
		{
			color *= texelFetch(sampler_border, ivec2(gl_FragCoord.xy), 0).r;
		}
	}
	
	
	color_out = vec4(color, 1.0f);
	
}