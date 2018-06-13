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

#include "cgogn/rendering/opengl/shader_param.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			ShaderParam::ShaderParam(ShaderProgram* program)
			{
				this->program = program;
				vao_ = cgogn::make_unique<cgogn::rendering::ogl::VAO>();
			}

			ShaderProgram* ShaderParam::get_shader()
			{
				return program;
			}

			void ShaderParam::bind()
			{
				program->bind();
				set_uniforms(); 
				vao_->bind(); 
			}

			void ShaderParam::bind_vao_only(bool with_uniforms)
			{
				if (with_uniforms)
					set_uniforms();
				vao_->bind();
			}

			void ShaderParam::release_vao_only()
			{
				vao_->release();
			}


			void ShaderParam::bind(const Matrix4f& proj, const Matrix4f& mv)
			{
				program->bind();
				program->set_matrices(proj, mv);
				set_uniforms();
				vao_->bind();
			}

			void ShaderParam::bind(float* proj, float *mv)
			{
				bind(Matrix4f(proj), Matrix4f(mv));
			}

		
			void ShaderParam::release()
			{
				program->release();
				vao_->release(); 
			}
		}
	}
}