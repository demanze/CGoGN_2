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

#include "cgogn/rendering/opengl/vao.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			VAO::VAO()
			{
				glGenVertexArrays(1, &handle);
			}

			void VAO::bind()
			{
				glBindVertexArray(handle);
			}

			void VAO::attribPointer(GLuint index, VBO* vbo, GLenum type)
			{
				vbo->bind();
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, vbo->vector_dimension(), type, GL_FALSE, 0, 0);
				vbo->release();
			}

			void VAO::attribPointer(GLuint index, VBO* vbo, GLenum type, GLuint divisor)
			{
				vbo->bind();
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, vbo->vector_dimension(), type, GL_FALSE, 0, 0);
				glVertexAttribDivisor(index, divisor);
				vbo->release();
			}

			void VAO::attribPointer(GLuint index, VBO* vbo, GLenum type, GLsizei stride, const void* pointer)
			{
				vbo->bind();
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, vbo->vector_dimension(), type, GL_FALSE, stride, pointer);
				vbo->release();
			}

			void VAO::release()
			{
				glBindVertexArray(0);
			}

			VAO::~VAO()
			{
				glDeleteVertexArrays(1, &handle);
			}
		}
	}
}
