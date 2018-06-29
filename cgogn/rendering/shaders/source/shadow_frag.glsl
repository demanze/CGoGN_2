#version 330 core

layout(location = 0) out float light_intensity;

uniform sampler2D shadowMap; 

in vec4 shadowPos;

void main()
{
	light_intensity = 1.0f; 
	if (texture(shadowMap, shadowPos.xy).r < shadowPos.z - 0.005f)
	{
		light_intensity = 0.5f;
	}
}