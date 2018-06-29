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
		Blur* Blur::instance_ = nullptr;

		Blur::Blur()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "fullscreen_texture_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "blur_frag.glsl");
			
			link();

			bind();

			unif_rgba_texture_sampler = "rgba_texture";
			unif_depth_filter = "depth_filter"; 
			unif_blur_dimension = "dimension";

			release(); 
		}

		void ParamBlur::set_rgba_sampler(GLint value)
		{
			Blur* sh = static_cast<Blur*>(this->program);
			sh->unif_rgba_texture_sampler.set(value);
		}

		void ParamBlur::set_depth_filter(GLint value)
		{
			Blur* sh = static_cast<Blur*>(this->program);
			sh->unif_depth_filter.set(value);
		}

		void ParamBlur::set_blur_dimension(GLuint value)
		{
			Blur* sh = static_cast<Blur*>(this->program);
			sh->unif_blur_dimension.set(value);  
		}

		std::unique_ptr< Blur::Param> Blur::generate_param()
		{
			if (!instance_)
			{
				instance_ = new Blur();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamBlur::set_uniforms()
		{

		}

		ParamBlur::ParamBlur(Blur* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 