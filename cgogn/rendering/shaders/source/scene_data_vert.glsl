#version 330 core

in vec3 vertex_pos; 
in vec3 vertex_normal; 

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;
uniform mat3 normal_matrix; 

out vec3 modelViewPosition; 
out vec3 modelViewNormal;

void main()
{
	vec4 model_view_position = model_view_matrix * vec4(vertex_pos,1.0f);
	
	modelViewPosition = model_view_position.xyz; 
	modelViewNormal = normal_matrix *vertex_normal; 
	
	gl_Position = projection_matrix * model_view_position;
}