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

#include <cgogn/rendering/shaders/shader_shadow.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
		Shadow* Shadow::instance_ = nullptr;

		Shadow::Shadow()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "shadow_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "shadow_frag.glsl");
			
			bindAttributeLocation("vertex_pos", ATTRIB_POS);
			bindAttributeLocation("vertex_norm", ATTRIB_NORM);

			link();

			bind();

			unif_shadowMap_ = "shadowMap";
			unif_shadowMVP_ = "shadowMVP";
			unif_pixelSize_ = "pixelSize";

			get_matrices_uniforms();

			release(); 
		}

		std::unique_ptr< Shadow::Param> Shadow::generate_param()
		{
			if (!instance_)
			{
				instance_ = new Shadow();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamShadow::set_shadowMap(GLint value)
		{
			auto sh = static_cast<Shadow*>(this->program);
			sh->unif_shadowMap_.set(value);
		}

		void ParamShadow::set_shadowMVP(float* value)
		{
			auto sh = static_cast<Shadow*>(this->program);
			sh->unif_shadowMVP_.set(Matrix4f(value));
		}

		void ParamShadow::set_pixelSize(float value)
		{
			auto sh = static_cast<Shadow*>(this->program);
			sh->unif_pixelSize_.set(value);
		}

		void ParamShadow::set_uniforms()
		{

		}

		ParamShadow::ParamShadow(Shadow* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 