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

#ifndef CGOGN_RENDERING_SHADER_PP1_H_
#define CGOGN_RENDERING_SHADER_PP1_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/vbo.h>


namespace cgogn
{

namespace rendering
{

namespace shaders2
{

	// forward
	class PostProcessing1;

	class CGOGN_RENDERING_API ParamPostProcessing1 : public ogl::ShaderParam
	{
		public:
			using Type = PostProcessing1;

			ParamPostProcessing1(PostProcessing1* sh);

			void set_rgba_sampler(GLint value);
			void set_blur_dimension(GLuint value);
			void set_pixel_size(float32 value);

			void set_uniforms(); 
	};

	class CGOGN_RENDERING_API PostProcessing1 : public ogl::ShaderProgram
	{
		friend class ParamPostProcessing1;
	
		private:
			static PostProcessing1* instance_;
			PostProcessing1();

		protected:
			static const char* vertex_shader_source_;
			static const char* fragment_shader_source_;

			// uniforms
			ogl::Uniform unif_rgba_texture_sampler;
			ogl::Uniform unif_blur_dimension; 
			ogl::Uniform unif_pixel_size; 
			
		public: 
			using Param = ParamPostProcessing1;
			using Self = PostProcessing1;
			CGOGN_NOT_COPYABLE_NOR_MOVABLE(PostProcessing1);

			static std::unique_ptr<Param> generate_param();
	};

}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
