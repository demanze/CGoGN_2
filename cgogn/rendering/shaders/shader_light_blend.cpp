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

#include <cgogn/rendering/shaders/shader_light_blend.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
	namespace LightBlend
	{
		Shader* Shader::instance_ = nullptr;

		Shader::Shader()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "fullscreen_texture_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "light_blend_frag.glsl");

			link();

			bind();

			unif_sampler_color = "sampler_color";
			unif_sampler_lighting = "sampler_lighting";
			unif_sampler_border = "sampler_border";

			release();
		}

		void Param::set_sampler_color(GLint value)
		{
			shader()->unif_sampler_color.set(value);
		}

		void Param::set_sampler_light(GLint value)
		{
			shader()->unif_sampler_lighting.set(value);
		}

		void Param::set_sampler_border(GLint value)
		{
			shader()->unif_sampler_border.set(value);
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