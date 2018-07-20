#version 330 core

layout(location = 0) out vec3 position_out;
layout(location = 1) out vec3 normal_out;

in vec3 modelViewPosition; 
in vec3 modelViewNormal; 

void main()
{ 
	position_out = modelViewPosition;
	normal_out = normalize(modelViewNormal); 
}

