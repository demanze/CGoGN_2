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
				target = ~0u;
			}

			GLuint Texture::id()
			{
				return handle;
			}

			GLuint Texture::slot()
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

			void Texture::setImage2D_simple(int w, int h, GLenum internalFormat, GLenum format, GLenum type)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, nullptr);
			}

			void Texture::setImage2D(const QImage& image)
			{
				QImage glImage = image.convertToFormat(QImage::Format_Grayscale8, Qt::NoOpaqueDetection);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, glImage.width(), glImage.height(), 0, GL_RED, GL_UNSIGNED_BYTE, glImage.scanLine(0));
				//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, glImage.width(), glImage.height(), GL_RGBA, GL_UNSIGNED_BYTE, glImage.constBits());
			}

			Texture::~Texture()
			{
				glDeleteTextures(1, &handle);
			}
		}
	}
}