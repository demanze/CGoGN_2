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

#ifndef CGOGN_RENDERING_SHADER_BLUR_H_
#define CGOGN_RENDERING_SHADER_BLUR_H_

#include <cgogn/rendering/opengl/all.h>



namespace cgogn
{

namespace rendering
{

namespace shaders
{

	// forward
	class Blur;

	class CGOGN_RENDERING_API ParamBlur : public ogl::ShaderParam
	{
		public:
			using Type = Blur;

			ParamBlur(Blur* sh);

			void set_blurred(GLint value);
			void set_depth_filter(GLint value);
			void set_blur_dimension(GLuint value);

			void set_uniforms(); 
	};

	class CGOGN_RENDERING_API Blur : public ogl::ShaderProgram
	{
		friend class ParamBlur;
	
		private:
			static Blur* instance_;
			Blur();

		protected:

			// uniforms
			ogl::Uniform unif_rgba_texture_sampler;
			ogl::Uniform unif_depth_filter;
			ogl::Uniform unif_blur_dimension; 
			
		public: 
			using Param = ParamBlur;
			using Self = Blur;
			CGOGN_NOT_COPYABLE_NOR_MOVABLE(Blur);

			static std::unique_ptr<Param> generate_param();
	};

}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
