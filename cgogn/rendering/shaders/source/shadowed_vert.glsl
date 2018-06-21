#version 330 core

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

uniform mat4 shadowMVP; 

out vec4 shadowPos; 

void main()
{
	float plane_size = 30.0f; 
	
	vec4 position = vec4(0.0f, 0.0f, -20.0f, 1.0f); 
	
	if (gl_VertexID == 0)
	{
		position.xyz += vec3(-1.0f, -1.0f, 0.0f)*plane_size;
	} 
	else if (gl_VertexID == 1) 
	{
		position.xyz += vec3(1.0f, -1.0f, 0.0f)*plane_size;
	} 
	else if (gl_VertexID == 2)
	{
		position.xyz += vec3(1.0f, 1.0f, 0.0f)*plane_size;
	} 
	else if (gl_VertexID == 3) 
	{
		position.xyz += vec3(1.0f, 1.0f, 0.0f)*plane_size;
	} 
	else if (gl_VertexID == 4)
	{
		position.xyz += vec3(-1.0f, 1.0f, 0.0f)*plane_size;
	} 
	else if (gl_VertexID == 5)
	{
		position.xyz += vec3(-1.0f, -1.0f, 0.0f)*plane_size;
	}
	
	vec4 model_view_position = model_view_matrix * position;
	gl_Position = projection_matrix * model_view_position;
	
	shadowPos = shadowMVP * position; 
 
}