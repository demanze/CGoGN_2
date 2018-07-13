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

#include "cgogn/rendering/opengl/framebuffer.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			GLint Framebuffer::qtDefaultFramebuffer = 0;

			Framebuffer::Framebuffer()
			{
				glGenFramebuffers(1, &handle);
			}

			void Framebuffer::bind()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, handle);
			}

			void Framebuffer::drawBuffers(GLenum _0)
			{
				GLenum drawBuffers[1] = { _0 };
				glDrawBuffers(1, drawBuffers);
			}

			void Framebuffer::drawBuffers(GLenum _0, GLenum _1)
			{
				GLenum drawBuffers[2] = { _0, _1 };
				glDrawBuffers(2, drawBuffers);
			}

			void Framebuffer::drawBuffers(GLenum _0, GLenum _1, GLenum _2)
			{
				GLenum drawBuffers[3] = { _0, _1, _2 };
				glDrawBuffers(3, drawBuffers);
			}

			void Framebuffer::drawBuffers(GLenum _0, GLenum _1, GLenum _2, GLenum _3)
			{
				GLenum drawBuffers[4] = { _0, _1, _2, _3 };
				glDrawBuffers(4, drawBuffers);
			}

			void Framebuffer::drawBuffers(GLenum _0, GLenum _1, GLenum _2, GLenum _3, GLenum _4)
			{
				GLenum drawBuffers[5] = { _0, _1, _2, _3, _4 };
				glDrawBuffers(5, drawBuffers);
			}

			void Framebuffer::release()
			{
				glBindFramebuffer(GL_FRAMEBUFFER, qtDefaultFramebuffer);
			}

			void Framebuffer::attach(std::unique_ptr<Texture>& texture, GLenum attachmentPoint)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texture->id(), 0);
				textures.push_back(&texture); 
			}

			std::unique_ptr<Texture>& Framebuffer::texture(uint32_t index)
			{
				return *textures[index]; 
			}

			void Framebuffer::check()
			{
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cgogn_log_info("framebuffer status") << "FBO is invalid.";
				}
			}

			Framebuffer::~Framebuffer()
			{
				glDeleteFramebuffers(1, &handle);
			}
		}
	}
}