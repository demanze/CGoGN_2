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

#ifndef OGL_SHADERPROGRAM
#define OGL_SHADERPROGRAM

#include "cgogn/rendering/reimp.h"
#include "shader_uniform.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			class CGOGN_RENDERING_API ShaderProgram
			{
				friend class Uniform;

				private:
					static std::vector<ShaderProgram*>* instances_;
				
					GLuint handle;
					std::vector<GLuint> shaders;

					Uniform unif_mv_matrix_;
					Uniform unif_projection_matrix_;
					Uniform unif_normal_matrix_;

				public:

					static void register_instance(ShaderProgram* sh);

					ShaderProgram();

					void addShader(GLenum type, const GLchar* source);

					void addShaderFromFile(GLenum type, const char* filename);

					void link();

					void bind();

					void release();

					void get_matrices_uniforms();

					void set_matrices(const Matrix4f& proj, const Matrix4f& mv);

					void set_view_matrix(const Matrix4f& mv);

					void bindAttributeLocation(const GLchar* name, GLuint index);

					static void clean_all();

					~ShaderProgram();
			};
		}
	}
}

#endif