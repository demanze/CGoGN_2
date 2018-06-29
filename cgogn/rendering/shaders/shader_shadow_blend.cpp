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

#include <cgogn/rendering/shaders/shader_shadow_blend.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
		ShadowBlend* ShadowBlend::instance_ = nullptr;

		ShadowBlend::ShadowBlend()
		{
			
			addShaderFromFile(GL_VERTEX_SHADER, "fullscreen_texture_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "shadow_blend_frag.glsl");
			
			link();

			bind();

			unif_texture_sampler1 = "texture_sampler1";
			unif_texture_sampler2 = "texture_sampler2";

			release(); 
		}

		void ParamShadowBlend::set_rgba_sampler1(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_texture_sampler1.set(value);
		}

		void ParamShadowBlend::set_rgba_sampler2(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_texture_sampler2.set(value);
		}

		std::unique_ptr< ShadowBlend::Param> ShadowBlend::generate_param()
		{
			if (!instance_)
			{
				instance_ = new ShadowBlend();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamShadowBlend::set_uniforms()
		{

		}

		ParamShadowBlend::ParamShadowBlend(ShadowBlend* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 