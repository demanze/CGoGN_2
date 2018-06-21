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

#define CGOGN_RENDER_SHADERS_FLAT_CPP_

#include <iostream>

#include <cgogn/rendering/shaders/shader_flat.h>

namespace cgogn
{

namespace rendering
{

const char* ShaderFlatGen::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"out vec3 pos;\n"
"void main()\n"
"{\n"
"	vec4 pos4 = model_view_matrix * vec4(vertex_pos,1.0);\n"
"	pos = pos4.xyz;"
"   gl_Position = projection_matrix * pos4;\n"
"}\n";

const char* ShaderFlatGen::fragment_shader_source_ =
"#version 150\n"
"out vec4 fragColor;\n"
"uniform vec4 front_color;\n"
"uniform vec4 back_color;\n"
"uniform vec4 ambiant_color;\n"
"uniform vec3 lightPosition;\n"
"uniform bool cull_back_face;\n"
"in vec3 pos;\n"
"void main()\n"
"{\n"
"	vec3 N = normalize(cross(dFdx(pos),dFdy(pos)));\n"
"	vec3 L = normalize(lightPosition-pos);\n"
"	float lambert = dot(N,L);\n"
"	if (gl_FrontFacing)\n"
"		fragColor = vec4(ambiant_color.rgb+lambert*front_color.rgb, front_color.a);\n"
//"		fragColor = ambiant_color+lambert*front_color;\n"
"	else\n"
"		if (cull_back_face) discard;\n"
//"		else fragColor = ambiant_color+lambert*back_color;\n"
"		else fragColor = vec4(ambiant_color.rgb+lambert*back_color.rgb, back_color.a);\n"
"}\n";

const char* ShaderFlatGen::vertex_shader_source2_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_col;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"out vec3 pos;\n"
"out vec3 col;\n"
"void main()\n"
"{\n"
"	vec4 pos4 = model_view_matrix * vec4(vertex_pos,1.0);\n"
"	pos = pos4.xyz;\n"
"	col = vertex_col;\n"
"   gl_Position = projection_matrix * pos4;\n"
"}\n";

const char* ShaderFlatGen::fragment_shader_source2_ =
"#version 150\n"
"out vec4 fragColor;\n"
"uniform vec4 ambiant_color;\n"
"uniform vec3 lightPosition;\n"
"uniform bool cull_back_face;\n"
"in vec3 pos;\n"
"in vec3 col;\n"
"void main()\n"
"{\n"
"	vec3 N = normalize(cross(dFdx(pos),dFdy(pos)));\n"
"	vec3 L = normalize(lightPosition-pos);\n"
"	float lambert = dot(N,L);\n"
"	if (gl_FrontFacing)\n"
"		fragColor = ambiant_color+vec4(lambert*col,1.0);\n"
"	else\n"
"		if (cull_back_face) discard;\n"
"		else fragColor = ambiant_color-vec4(lambert*col,1.0);\n"
"}\n";

ShaderFlatGen::ShaderFlatGen(bool color_per_vertex)
{
	if (color_per_vertex)
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source2_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source2_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		bindAttributeLocation("vertex_col", ATTRIB_COLOR);
		link();

		bind();

		get_matrices_uniforms();
	}
	else
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		link();

		bind();

		get_matrices_uniforms();
	} 

	unif_front_color_ = "front_color";
	unif_back_color_ = "back_color";
	unif_ambiant_color_ = "ambiant_color";
	unif_light_position_ = "lightPosition";
	unif_bf_culling_ = "cull_back_face";

	release(); 
}

void ShaderFlatGen::set_light_position(const Vector3f& l)
{
	unif_light_position_.set(l);
}

void ShaderFlatGen::set_local_light_position(const Vector3f& l, const Matrix4f& view_matrix)
{
	Vector4f loc4 = view_matrix * Vector4f(l.x(), l.y(), l.z(), 1.0);
	unif_light_position_.set(Vector3f(loc4.head<3>() / loc4.w()));
}

void ShaderFlatGen::set_front_color(const Color& rgb)
{
	if (unif_front_color_.found())
		unif_front_color_.set(rgb);
}

void ShaderFlatGen::set_back_color(const Color& rgb)
{
	if (unif_back_color_.found())
		unif_back_color_.set(rgb);
}

void ShaderFlatGen::set_ambiant_color(const Color& rgb)
{
	unif_ambiant_color_.set(rgb);
}

void ShaderFlatGen::set_bf_culling(bool cull)
{
	unif_bf_culling_.set(cull);
}

template class CGOGN_RENDERING_API ShaderFlatTpl<false>;
template class CGOGN_RENDERING_API ShaderFlatTpl<true>;
template class CGOGN_RENDERING_API ShaderParamFlat<false>;
template class CGOGN_RENDERING_API ShaderParamFlat<true>;

} // namespace rendering

} // namespace cgogn
