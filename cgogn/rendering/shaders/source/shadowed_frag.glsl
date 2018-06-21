#version 330 core

layout(location = 0) out vec4 color_out;

uniform sampler2D shadowMap; 

in vec4 shadowPos;

void main()
{
	float lighting = 1.0f; 
	if (texture(shadowMap, shadowPos.xy).r < shadowPos.z - 0.005f)
	{
		lighting = 0.5f;
	}
	color_out = vec4(vec3(0.5f,0.5f,0.5f)*lighting,1.0f);
}