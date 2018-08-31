#version 330 core

in vec3 vertex_pos; 

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

uniform mat4 shadowMVP; 

void main()
{
	vec4 model_view_position = model_view_matrix * vec4(vertex_pos,1.0f);
	
	gl_Position = projection_matrix * model_view_position;
}