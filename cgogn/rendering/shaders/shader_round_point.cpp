/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#define CGOGN_RENDER_SHADERS_ROUND_POINT_CPP_

#include <iostream>

#include <cgogn/rendering/shaders/shader_round_point.h>


namespace cgogn
{

namespace rendering
{

const char* ShaderRoundPointGen::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"void main()\n"
"{\n"
"   gl_Position =  vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderRoundPointGen::geometry_shader_source_ =
"#version 150\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices=4) out;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 pointSizes;\n"
"uniform vec4 plane_clip;\n"
"uniform vec4 plane_clip2;\n"
"out vec2 local;\n"
"void main()\n"
"{\n"
"	float d = dot(plane_clip,gl_in[0].gl_Position);\n"
"	float d2 = dot(plane_clip2,gl_in[0].gl_Position);\n"
"	if ((d<=0.0)&&(d2<=0.0))\n"
"	{\n"
"		vec4 A = projection_matrix*model_view_matrix * gl_in[0].gl_Position;\n"
"		A = A/A.w;\n"
"		local = vec2(-1.0,-1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(-pointSizes[0],-pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(1.0,-1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(pointSizes[0],-pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(-1.0,1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(-pointSizes[0],pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(1.0,1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(pointSizes[0],pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		EndPrimitive();\n"
"	}\n"
"}\n";

const char* ShaderRoundPointGen::fragment_shader_source_ =
"#version 150\n"
"uniform vec4 color;\n"
"in vec2 local;\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"

"	float r2 = dot(local,local);\n"
"   if (r2 > 1.0) discard;\n"
"   fragColor = vec4(color.rgb,(1.0-r2*r2));\n"
"}\n";

const char* ShaderRoundPointGen::vertex_shader_source2_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_color;\n"
"out vec3 color_v;\n"
"void main()\n"
"{\n"
"   color_v = vertex_color;\n"
"   gl_Position = vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderRoundPointGen::geometry_shader_source2_ =
"#version 150\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices=4) out;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 pointSizes;\n"
"uniform vec4 plane_clip;\n"
"uniform vec4 plane_clip2;\n"
"in vec3 color_v[];\n"
"out vec2 local;\n"
"out vec3 color_f;\n"
"void main()\n"
"{\n"
"	float d = dot(plane_clip,gl_in[0].gl_Position);\n"
"	float d2 = dot(plane_clip2,gl_in[0].gl_Position);\n"
"	if ((d<=0.0)&&(d2<=0.0))\n"
"	{\n"
"		vec4 A = projection_matrix*model_view_matrix * gl_in[0].gl_Position;\n"
"		A = A/A.w;\n"
"		color_f = color_v[0];\n"
"		local = vec2(-1.0,-1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(-pointSizes[0],-pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(1.0,-1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(pointSizes[0],-pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(-1.0,1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(-pointSizes[0],pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		local = vec2(1.0,1.0);\n"
"		gl_Position = vec4(A.xyz-vec3(pointSizes[0],pointSizes[1],0.0), 1.0);\n"
"		EmitVertex();\n"
"		EndPrimitive();\n"
"	}\n"
"}\n";

const char* ShaderRoundPointGen::fragment_shader_source2_ =
"#version 150\n"
"in vec2 local;\n"
"in vec3 color_f;\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	float r2 = dot(local,local);\n"
"   if (r2 > 1.0) discard;\n"
"   fragColor = vec4(color_f,(1.0-r2*r2));\n"
"}\n";

ShaderRoundPointGen::ShaderRoundPointGen(bool color_per_vertex)
{
	if (color_per_vertex)
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source2_);
		addShader(GL_GEOMETRY_SHADER, geometry_shader_source2_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source2_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		bindAttributeLocation("vertex_color", ATTRIB_COLOR);
		link();
	}
	else
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source_);
		addShader(GL_GEOMETRY_SHADER, geometry_shader_source_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		link();
	}

	bind(); 

	get_matrices_uniforms();

	unif_color_ = "color";
	unif_size_ = "pointSizes";
	unif_plane_clip_ = "plane_clip";
	unif_plane_clip2_ = "plane_clip2";

	set_size(3.0f);
	set_color(Color(255, 255, 255));
	set_plane_clip(Vector4f(0,0,0,0));
	set_plane_clip2(Vector4f(0,0,0,0));

	release(); 
}

void ShaderRoundPointGen::set_color(const Color& rgb)
{
	if (unif_color_.found())
		unif_color_.set(rgb);
}

void ShaderRoundPointGen::set_size(float32 wpix)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	Vector2f wd(wpix / float32(viewport[2]), wpix / float32(viewport[3]));
	unif_size_.set(wd);
}


void ShaderRoundPointGen::set_plane_clip(const Vector4f& plane)
{
	unif_plane_clip_.set(plane);
}

void ShaderRoundPointGen::set_plane_clip2(const Vector4f& plane)
{
	unif_plane_clip2_.set(plane);
}

template class CGOGN_RENDERING_API ShaderRoundPointTpl<false>;
template class CGOGN_RENDERING_API ShaderRoundPointTpl<true>;
template class CGOGN_RENDERING_API ShaderParamRoundPoint<false>;
template class CGOGN_RENDERING_API ShaderParamRoundPoint<true>;


} // namespace rendering

} // namespace cgogn
