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

#ifndef OGL_TEXTURE
#define OGL_TEXTURE

#include "cgogn/rendering/reimp.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			class CGOGN_RENDERING_API Texture
			{
				private:
					GLuint handle;

					//The texture slot to which the texture is currently bound (0 for GL_TEXTURE0, 1 for GL_TEXTURE1...) 
					//Can be provided to a shader to use the texture as a sampler 
					GLuint target; 

				public: 

					Texture();

					GLuint id();

					GLuint slot();

					void bind();

					void bindAt(GLint slot);

					void releaseAt(GLint slot);

					void release();

					void setImage2D_simple(int w, int h, GLenum internalFormat, GLenum format, GLenum type);
					
					void setImage2D_repeat(int w, int h, GLenum internalFormat, GLenum format, GLenum type, void* data);

					void setImage2D(const QImage& image);
					
					~Texture();
			};
		}
	}
}

#endif