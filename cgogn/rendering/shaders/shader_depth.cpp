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

#include <iostream>

#include <cgogn/rendering/shaders/shader_depth.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
		Depth* Depth::instance_ = nullptr;

		Depth::Depth()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "depth_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "depth_frag.glsl");
			
			bindAttributeLocation("vertex_pos", ATTRIB_POS);

			link();

			bind();

			get_matrices_uniforms();

			release(); 
		}

		std::unique_ptr< Depth::Param> Depth::generate_param()
		{
			if (!instance_)
			{
				instance_ = new Depth();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamDepth::set_uniforms()
		{

		}

		ParamDepth::ParamDepth(Depth* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 