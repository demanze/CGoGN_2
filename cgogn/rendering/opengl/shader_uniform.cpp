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

#include "cgogn/rendering/opengl/shader_uniform.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			GLuint Uniform::currentProgram = 0; 

			void Uniform::operator=(const GLchar* name)
			{
				location = glGetUniformLocation(currentProgram, name);
			}

			bool Uniform::found()
			{
				return (0 <= location);
			}

			void Uniform::set(GLint value)
			{
				glUniform1i(location, value);
			}

			void Uniform::set(GLuint value)
			{
				glUniform1ui(location, value);
			}
			
			void Uniform::set(GLfloat value)
			{
				glUniform1f(location, value);
			}

			void Uniform::set(Vector2f value)
			{
				glUniform2f(location, value.x(), value.y());
			}

			void Uniform::set(Vector3f value)
			{
				glUniform3f(location, value.x(), value.y(), value.z());
			}

			void Uniform::set(Vector4f value)
			{
				glUniform4f(location, value.x(), value.y(), value.z(), value.w());
			}

			void Uniform::set(Color value)
			{
				glUniform4f(location, value[0], value[1], value[2], value[3]);
			}

			void Uniform::set(Matrix4f value)
			{
				glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
			}

			void Uniform::set(Matrix3f value)
			{
				glUniformMatrix3fv(location, 1, GL_FALSE, value.data());
			}

			void Uniform::set(bool value)
			{
				glUniform1i(location, value ? 1 : 0);
			}
		}
	}
}
