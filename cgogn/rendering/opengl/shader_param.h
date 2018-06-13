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

#ifndef OGL_SHADERPARAM
#define OGL_SHADERPARAM

#include "shader_program_.h"
#include "vao.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			class CGOGN_RENDERING_API ShaderParam
			{
				protected: 
					ShaderProgram* program; 
					ShaderParam(ShaderProgram* program);
					std::unique_ptr<cgogn::rendering::ogl::VAO> vao_;

					virtual void set_uniforms() = 0;

				public:
					ShaderProgram* get_shader();

					void bind();

					void bind_vao_only(bool with_uniforms = true);

					void release_vao_only();

					void bind(const Matrix4f& proj, const Matrix4f& mv);
					void bind(float* proj, float* mv);

					void release();
			};
		}
	}
}

#endif