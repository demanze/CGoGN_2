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

#include "cgogn/rendering/opengl/buffer.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			Buffer::Buffer()
			{
				this->target_ = GL_ARRAY_BUFFER; 
			}

			Buffer::Buffer(GLenum target)
			{
				this->target_ = target;
			}

			GLuint Buffer::bufferId() const
			{
				return handle;
			}

			void Buffer::create()
			{
				glGenBuffers(1, &handle); 
			}

			void Buffer::destroy()
			{
				glDeleteBuffers(1, &handle);
				handle = 0;
			}

			void Buffer::setUsagePattern(GLenum usage)
			{
				this->usage_ = usage;
			}

			void Buffer::bind()
			{
				glBindBuffer(target_, handle);
			}

			void Buffer::release()
			{
				glBindBuffer(target_, 0);
			}

			void Buffer::allocate(GLsizeiptr size)
			{
				glBufferData(target_, size, nullptr, usage_);
			}

			void Buffer::allocate(void* data, GLsizeiptr size)
			{
				glBufferData(target_, size, data, usage_);
			}

			void Buffer::write(GLintptr offset, const void* src, GLsizeiptr size)
			{
				glBufferSubData(target_, offset, size, src);
			}

			GLvoid* Buffer::map(GLenum access)
			{
				return glMapBuffer(target_, access);
			}

			void Buffer::unmap()
			{
				glUnmapBuffer(target_);
			}

			bool Buffer::isCreated()
			{
				return (handle != 0);
			}
		}
	}
}
