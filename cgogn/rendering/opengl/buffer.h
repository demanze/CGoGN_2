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

#ifndef OGL_BUFFER_H
#define OGL_BUFFER_H

#include "cgogn/rendering/reimp.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			class CGOGN_RENDERING_API Buffer
			{
				private:
					GLuint handle = 0; 
					GLenum target_;
					GLenum usage_;

				public: 

					Buffer();

					Buffer(GLenum target);

					GLuint bufferId() const;

					void create();

					void destroy();

					bool isCreated(); 

					void setUsagePattern(GLenum usage);

					void bind();

					void release();

					void allocate(GLsizeiptr size);

					void allocate(void* data, GLsizeiptr size);

					void write(GLintptr offset, const void* src, GLsizeiptr size);

					GLvoid* map(GLenum access);

					void unmap();

			};
		}
	}
}

#endif