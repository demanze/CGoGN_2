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


#include <cgogn/rendering/shaders/shader_texture.h>
#include <iostream>

namespace cgogn
{

namespace rendering
{

ShaderTexture* ShaderTexture::instance_ = nullptr;

const char* ShaderTexture::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec2 vertex_tc;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"out vec2 tc;\n"
"void main()\n"
"{\n"
"	tc = vertex_tc;\n"
"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderTexture::fragment_shader_source_ =
"#version 150\n"
"out vec4 frag_color;\n"
"uniform sampler2D texture_unit;\n"
"in vec2 tc;\n"
"void main()\n"
"{\n"
"	frag_color = texture(texture_unit,tc);\n"
"}\n";

ShaderTexture::ShaderTexture()
{
	addShader(GL_VERTEX_SHADER, vertex_shader_source_);
	addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);

	bindAttributeLocation("vertex_pos", ATTRIB_POS);
	bindAttributeLocation("vertex_tc", ATTRIB_TC);

	link();

	bind(); 
	get_matrices_uniforms();

	ogl::Uniform unif_texture_unit_; 
	unif_texture_unit_ = "texture_unit";
	unif_texture_unit_.set(0); 

	release(); 
}

ShaderParamTexture::ShaderParamTexture(ShaderTexture* sh) :
	ogl::ShaderParam(sh),
	texture_(nullptr)
{}

void ShaderParamTexture::set_uniforms()
{
	if (texture_)
	{
		glActiveTexture(GL_TEXTURE0);
		texture_->bind();
	}
}

void ShaderParamTexture::set_vbo(VBO* vbo_pos, VBO* vbo_tc)
{
	program->bind();
	vao_->bind();
	vao_->attribPointer(ShaderTexture::ATTRIB_POS, vbo_pos, GL_FLOAT);
	vao_->attribPointer(ShaderTexture::ATTRIB_TC, vbo_tc, GL_FLOAT);
	vao_->release();
	program->release();
}

} // namespace rendering

} // namespace cgogn
