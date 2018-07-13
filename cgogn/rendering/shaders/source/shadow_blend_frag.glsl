#version 330

layout(location = 0) out vec4 color_out;

uniform sampler2D sampler_scene_color;
uniform sampler2D sampler_scene_position;
uniform sampler2D sampler_scene_normal;

uniform sampler2D sampler_shadow;

uniform mat4 projection_matrix; 
uniform vec3 ssao_kernel[64];

uniform bool enableShadow;
uniform bool enableSSAO; 

void main()
{
	vec3 color = texelFetch(sampler_scene_color, ivec2(gl_FragCoord.xy), 0).rgb; 
	
	if (enableShadow) 
	{
		float shadowMask = texelFetch(sampler_shadow, ivec2(gl_FragCoord.xy), 0).r; 
		color *= shadowMask;
	}
	
	if (enableSSAO) 
	{
		vec3 positionModelView = texelFetch(sampler_scene_position, ivec2(gl_FragCoord.xy), 0).xyz; 
		vec4 positionProjection = projection_matrix * vec4(positionModelView, 1.0f); 
		positionProjection /= positionProjection.w; 
		positionProjection = (positionProjection * 0.5f) + 0.5f; 
			 
		float radius = 5.0f; 
		float bias = 0.01f;
	
		float occlusion = 0.0f; 
		for (int i=0;i<64;i++) 
		{
			 vec3 sampleModelView = positionModelView + (ssao_kernel[i] * radius); 
			 
			 vec4 sampleProjection = projection_matrix * vec4(sampleModelView, 1.0f);
			 sampleProjection /= sampleProjection.w; 
			 sampleProjection = (sampleProjection * 0.5f) + 0.5f; 
			 
			 float sampleDepth = texture(sampler_scene_position, sampleProjection.xy).z; 
			 
			 float rangeCheck = smoothstep(0.0, 1.0, radius / abs(positionModelView.z - sampleDepth));
			 occlusion += ((sampleDepth >= (sampleModelView.z + bias)) ? 1.0f : 0.0f) * rangeCheck; 
		}
		
		occlusion = 1.0f - (occlusion / 64.0f);
		color *= occlusion; 
	}
	
	color_out = vec4(color, 1.0f);
}