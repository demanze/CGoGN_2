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

#define CGOGN_RENDER_SHADERS_PHONG_CPP_

#include <iostream>

#include <cgogn/rendering/shaders/shader_phong.h>

namespace cgogn
{

namespace rendering
{

const char* ShaderPhongGen::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_normal;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform mat3 normal_matrix;\n"
"uniform vec3 lightPosition;\n"
"out vec3 EyeVector;\n"
"out vec3 Normal;\n"
"out vec3 LightDir;\n"
"void main ()\n"
"{\n"
"	Normal = normal_matrix * vertex_normal;\n"
"	vec3 Position = vec3 (model_view_matrix * vec4 (vertex_pos, 1.0));\n"
"	LightDir = lightPosition - Position;\n"
"	EyeVector = -Position;"
"	gl_Position = projection_matrix * model_view_matrix * vec4 (vertex_pos, 1.0);\n"
"}\n";

const char* ShaderPhongGen::fragment_shader_source_ =
"#version 150\n"
"in vec3 EyeVector;\n"
"in vec3 Normal;\n"
"in vec3 LightDir;\n"
"uniform vec4 front_color;\n"
"uniform vec4 spec_color;\n"
"uniform vec4 ambiant_color;\n"
"uniform vec4 back_color;\n"
"uniform float spec_coef;\n"
"uniform bool double_side;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"	vec3 N = normalize (Normal);\n"
"	vec3 L = normalize (LightDir);\n"
"	vec4 finalColor = ambiant_color;\n"
"	vec4 currentColor = front_color;\n"
"	if (gl_FrontFacing==false)\n" // do not use ! because of bug on old intel under OS/X
"	{\n"
"		if (!double_side)\n"
"			discard;\n"
"		N *= -1.0;\n"
"		currentColor = back_color;\n"
"	}\n"
"	float lambertTerm = clamp(dot(N,L),0.0,1.0);\n"
"	finalColor += currentColor*lambertTerm ;\n"
"	vec3 E = normalize(EyeVector);\n"
"	vec3 R = reflect(-L, N);\n"
"	float specular = pow( max(dot(R, E), 0.0), spec_coef );\n"
"	finalColor += spec_color * specular;\n"
"	frag_color=finalColor;\n"
"}\n";

const char* ShaderPhongGen::vertex_shader_source_2_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_normal;\n"
"in vec3 vertex_color;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform mat3 normal_matrix;\n"
"uniform vec3 lightPosition;\n"
"out vec3 EyeVector;\n"
"out vec3 Normal;\n"
"out vec3 LightDir;\n"
"out vec3 front_color;\n"
"void main ()\n"
"{\n"
"	Normal = normal_matrix * vertex_normal;\n"
"	vec3 Position = vec3 (model_view_matrix * vec4 (vertex_pos, 1.0));\n"
"	LightDir = lightPosition - Position;\n"
"	EyeVector = -Position;"
"	front_color = vertex_color;"
"	gl_Position = projection_matrix * model_view_matrix * vec4 (vertex_pos, 1.0);\n"
"}\n";

const char* ShaderPhongGen::fragment_shader_source_2_ =
"#version 150\n"
"in vec3 EyeVector;\n"
"in vec3 Normal;\n"
"in vec3 LightDir;\n"
"in vec3 front_color;\n"
"uniform vec4 spec_color;\n"
"uniform vec4 ambiant_color;\n"
"uniform float spec_coef;\n"
"uniform bool double_side;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"	vec3 N = normalize (Normal);\n"
"	vec3 L = normalize (LightDir);\n"
"	vec4 finalColor = ambiant_color;\n"
"	if (gl_FrontFacing==false)\n" // do not use ! because of bug on old intel under OS/X
"	{\n"
"		if (!double_side)\n"
"			discard;\n"
"		N *= -1.0;\n"
"	}\n"
"	float lambertTerm = clamp(dot(N,L),0.0,1.0);\n"
"	finalColor += vec4(front_color*lambertTerm,0.0);\n"
"	vec3 E = normalize(EyeVector);\n"
"	vec3 R = reflect(-L, N);\n"
"	float specular = pow( max(dot(R, E), 0.0), spec_coef );\n"
"	finalColor += spec_color * specular;\n"
"	frag_color=finalColor;\n"
"}\n";



ShaderPhongGen::ShaderPhongGen(bool color_per_vertex)
{
	if (color_per_vertex)
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source_2_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source_2_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		bindAttributeLocation("vertex_normal", ATTRIB_NORM);
		bindAttributeLocation("vertex_color", ATTRIB_COLOR);
		link();

		bind();

		get_matrices_uniforms();
	}
	else
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		bindAttributeLocation("vertex_normal", ATTRIB_NORM);
		link();

		bind();

		get_matrices_uniforms();
	}

	unif_front_color_ = "front_color";
	unif_back_color_ = "back_color";
	unif_ambiant_color_ = "ambiant_color";
	unif_spec_color_ ="spec_color";
	unif_spec_coef_ = "spec_coef";
	unif_double_side_ = "double_side";
	unif_light_position_ = "lightPosition";

	//default param

	set_light_position(Vector3f(10.0f, 100.0f, 1000.0f));
	set_front_color(Color(250, 0, 0));
	set_back_color(Color(0, 250, 5));
	set_ambiant_color(Color(5, 5, 5));
	set_specular_color(Color(100, 100, 100));
	set_specular_coef(50.0f);
	set_double_side(true);
	release();
}

void ShaderPhongGen::set_light_position(const Vector3f& l)
{
	unif_light_position_.set(l);
}

void ShaderPhongGen::set_local_light_position(const Vector3f& l, const Matrix4f& view_matrix)
{
	Vector4f loc4 = view_matrix * Vector4f(l.x(), l.y(), l.z(), 1.0);
	unif_light_position_.set(Vector3f(loc4.head<3>() / loc4.w()));
}

void ShaderPhongGen::set_front_color(const Vector4f& rgb)
{
	if (unif_front_color_.found())
		unif_front_color_.set(rgb);
}

void ShaderPhongGen::set_back_color(const Vector4f& rgb)
{
	if (unif_back_color_.found())
		unif_back_color_.set(rgb);
}

void ShaderPhongGen::set_ambiant_color(const Vector4f& rgb)
{
	unif_ambiant_color_.set(rgb);
}

void ShaderPhongGen::set_specular_color(const Vector4f& rgb)
{
	unif_spec_color_.set(rgb);
}

void ShaderPhongGen::set_specular_coef(float32 coef)
{
	unif_spec_coef_.set(coef);
}

void ShaderPhongGen::set_double_side(bool ts)
{
	unif_double_side_.set(ts);
}

template class CGOGN_RENDERING_API ShaderPhongTpl<false>;
template class CGOGN_RENDERING_API ShaderPhongTpl<true>;
template class CGOGN_RENDERING_API ShaderParamPhong<false>;
template class CGOGN_RENDERING_API ShaderParamPhong<true>;

} // namespace rendering

} // namespace cgogn
