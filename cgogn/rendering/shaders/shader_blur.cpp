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

#include <cgogn/rendering/shaders/shader_blur.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
	namespace Blur
	{
		Shader* Shader::instance_ = nullptr;

		Shader::Shader()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "fullscreen_texture_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "blur_frag.glsl");

			link();

			bind();

			unif_rgba_texture_sampler = "rgba_texture";
			unif_position_texture = "position_texture";
			unif_blur_dimension = "dimension";
			unif_radius = "radius";

			release();
		}

		void Param::set_blurred(GLint value)
		{
			shader()->unif_rgba_texture_sampler.set(value);
		}

		void Param::set_position_texture(GLint value)
		{
			shader()->unif_position_texture.set(value);
		}

		void Param::set_blur_dimension(GLuint value)
		{
			shader()->unif_blur_dimension.set(value);
		}

		void Param::set_radius(GLfloat value)
		{
			shader()->unif_radius.set(value);
		}

		void Param::set_uniforms()
		{

		}

		std::unique_ptr<Param> Param::generate()
		{
			if (!Shader::instance_)
			{
				Shader::instance_ = new Shader();
				Shader::ShaderProgram::register_instance(Shader::instance_);
			}
			return cgogn::make_unique<Param>(Shader::instance_);
		}
	}
}

} 

} 