#version 330

out vec2 uv;

void main()
{
	if (gl_VertexID == 0)
	{
		gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		uv = vec2(0.0f,0.0f);
	} 
	else if (gl_VertexID == 1) 
	{
		gl_Position = vec4(1.0f, -1.0f, 0.0f, 1.0f);
		uv = vec2(1.0f,0.0f);
	} 
	else if (gl_VertexID == 2)
	{
		gl_Position = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		uv = vec2(1.0f,1.0f);
	} 
	else if (gl_VertexID == 3) 
	{
		gl_Position = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		uv = vec2(1.0f,1.0f);
	} 
	else if (gl_VertexID == 4)
	{
		gl_Position = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
		uv = vec2(0.0f,1.0f);
	} 
	else if (gl_VertexID == 5)
	{
		gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		uv = vec2(0.0f,0.0f);
	}
}