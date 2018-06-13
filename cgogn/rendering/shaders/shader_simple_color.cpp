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


#include <iostream>

#include <cgogn/rendering/shaders/shader_simple_color.h>

namespace cgogn
{

namespace rendering
{

ShaderSimpleColor* ShaderSimpleColor::instance_ = nullptr;

std::unique_ptr<ShaderSimpleColor::Param> ShaderSimpleColor::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderSimpleColor();
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}

const char* ShaderSimpleColor::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"void main()\n"
"{\n"
"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderSimpleColor::fragment_shader_source_ =
"#version 150\n"
"out vec4 fragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   fragColor = color;\n"
"}\n";

ShaderSimpleColor::ShaderSimpleColor()
{
	addShader(GL_VERTEX_SHADER, vertex_shader_source_);
	addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
	bindAttributeLocation("vertex_pos", ATTRIB_POS);

	link();

	bind();

	get_matrices_uniforms();

	unif_color_ = "color";

	// default param
	set_color(Color(255, 255, 255));

	release(); 
}

void ShaderSimpleColor::set_color(const Vector4f& rgb)
{
	unif_color_.set(rgb);
}

} // namespace rendering

} // namespace cgogn
