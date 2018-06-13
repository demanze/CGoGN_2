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

#include "cgogn/rendering/opengl/texture.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			Texture::Texture()
			{
				glGenTextures(1, &handle);
				target = GL_INVALID_ENUM;
			}

			GLuint Texture::id() const
			{
				return handle;
			}

			GLuint Texture::slot() const
			{
				return target;
			}

			void Texture::bind()
			{
				glBindTexture(GL_TEXTURE_2D, handle);
			}

			void Texture::bindAt(GLint slot)
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, handle);
				target = slot;
			}

			void Texture::releaseAt(GLint slot)
			{
				glActiveTexture(GL_TEXTURE0 + slot);
				glBindTexture(GL_TEXTURE_2D, 0);
				target = ~0u;
			}

			void Texture::release()
			{
				glBindTexture(target, 0);
				target = ~0u;
			}


			void Texture::setImage2D_simple(int w, int h, GLenum internal_format, GLenum format, GLenum type)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, type, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			Texture::~Texture()
			{
				glDeleteTextures(1, &handle);
			}
		}
	}
}