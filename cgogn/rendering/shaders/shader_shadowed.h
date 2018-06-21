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

#ifndef CGOGN_RENDERING_SHADER_SHADOWED_H_
#define CGOGN_RENDERING_SHADER_SHADOWED_H_

#include <cgogn/rendering/opengl/all.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{

	// forward
	class Shadowed;

	class CGOGN_RENDERING_API ParamShadowed : public ogl::ShaderParam
	{
		public:
			using Type = Shadowed;

			ParamShadowed(Shadowed* sh);

			void set_shadowMap(GLint value);
			void set_shadowMVP(float* value);

			void set_uniforms(); 
	};

	class CGOGN_RENDERING_API Shadowed : public ogl::ShaderProgram
	{
		friend class ParamShadowed;
	
		private:
			static Shadowed* instance_;
			Shadowed();

			ogl::Uniform unif_shadowMap_; 
			ogl::Uniform unif_shadowMVP_;

		public: 
			using Param = ParamShadowed;
			using Self = Shadowed;
			CGOGN_NOT_COPYABLE_NOR_MOVABLE(Shadowed);

			static std::unique_ptr<Param> generate_param();
	};

}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
