#version 330 core

layout(location = 0) out float lighting_out;

uniform sampler2D sampler_scene_position;
uniform sampler2D sampler_scene_normal;

uniform sampler2D sampler_noise;
uniform vec2 noiseScale;
const int ssaoSamples = 32; 

uniform mat4 projection_matrix; 
uniform vec3 ssao_kernel[ssaoSamples];

uniform float radiusSSAO; 

void main()
{
	vec3 positionModelView = texelFetch(sampler_scene_position, ivec2(gl_FragCoord.xy), 0).xyz; 
	vec3 normalModelView = texelFetch(sampler_scene_normal, ivec2(gl_FragCoord.xy), 0).xyz; 
	vec3 randomVec = texture(sampler_noise, vec2(gl_FragCoord.xy) * noiseScale, 0).xyz;  
	
	vec3 tangent   = normalize(randomVec - normalModelView * dot(randomVec, normalModelView));
	vec3 bitangent = cross(normalModelView, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normalModelView);  

	vec4 positionProjection = projection_matrix * vec4(positionModelView, 1.0f); 
	
	positionProjection /= positionProjection.w; 
	positionProjection = (positionProjection * 0.5f) + 0.5f; 
		
	float bias = 0.0001f;

	float occlusion = 0.0f; 
	for (int i=0;i<ssaoSamples;i++) 
	{
		 vec3 sampleModelView = positionModelView +  (TBN * (ssao_kernel[i] * radiusSSAO)); 
		 
		 vec4 sampleProjection = projection_matrix * vec4(sampleModelView, 1.0f);
		 sampleProjection /= sampleProjection.w; 
		 sampleProjection = (sampleProjection * 0.5f) + 0.5f; 
		 
		 float sampleDepth = texture(sampler_scene_position, sampleProjection.xy).z; 
		 
		 float dif = sampleDepth -  sampleModelView.z;
		 occlusion += ((0.0f <= dif) && (dif < radiusSSAO)) ? (1.0f / float(ssaoSamples)) : 0.0f; 
	}		
	
	lighting_out = (1.0f - occlusion); 
}

