#version 330

layout(location = 0) out vec4 color_out;

const float kernel[11] = float[11](0.084264, 0.088139, 0.091276, 0.093585, 0.094998, 0.095474, 0.094998, 0.093585, 0.091276, 0.088139, 0.084264);

const int kernelSize = 11;
uniform sampler2D rgba_texture;
uniform sampler2D depth_filter;
uniform uint dimension;

void main()
{
	float centerDepth = texelFetch(depth_filter, ivec2(gl_FragCoord.xy), 0).r;
	
	vec3 sum = vec3(0.0f,0.0f,0.0f);
	
	float totalKernelUsed = 0.0f; 
	for (int i=0;i<kernelSize;i++) 
	{
		ivec2 offset = ivec2(gl_FragCoord.xy);
		offset[dimension] += i - (kernelSize / 2);
		float depth = texelFetch(depth_filter, offset, 0).r; 
		if (abs(depth - centerDepth) < 0.0001f)
		{
			sum += texelFetch(rgba_texture, offset, 0).rgb*kernel[i];
			totalKernelUsed += kernel[i]; 
		}
	}
	
	color_out = vec4(sum / totalKernelUsed,1.0f);
}