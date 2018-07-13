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

#ifndef CGOGN_RENDERING_SHADER_FULLSCREEN_TEXTURE_H_
#define CGOGN_RENDERING_SHADER_FULLSCREEN_TEXTURE_H_

#include <cgogn/rendering/opengl/all.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{

	// forward
	class ShadowBlend;

	class CGOGN_RENDERING_API ParamShadowBlend : public ogl::ShaderParam
	{
		public:
			using Type = ShadowBlend;

			ParamShadowBlend(ShadowBlend* sh);

			void set_enable_shadow(bool value);
			void set_enable_ssao(bool value);

			void set_sampler_scene_color(GLint value);
			void set_sampler_shadow(GLint value);
			void set_sampler_scene_depth(GLint value);
			void set_sampler_scene_position(GLint value);
			void set_sampler_scene_normal(GLint value);

			void set_projection_matrix(Matrix4f value);

			void set_uniforms(); 
	};

	class CGOGN_RENDERING_API ShadowBlend : public ogl::ShaderProgram
	{
		friend class ParamShadowBlend;
	
		private:
			static ShadowBlend* instance_;
			ShadowBlend();

		protected:

			// uniforms
			ogl::Uniform unif_sampler_scene_color;
			ogl::Uniform unif_sampler_shadow;
			ogl::Uniform unif_sampler_scene_depth;
			ogl::Uniform unif_sampler_scene_position;
			ogl::Uniform unif_sampler_scene_normal;

			ogl::Uniform unif_ssao_kernel;
			ogl::Uniform unif_projection_matrix;

			ogl::Uniform unif_enable_shadow;
			ogl::Uniform unif_enable_ssao;

		public: 
			using Param = ParamShadowBlend;
			using Self = ShadowBlend;
			CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShadowBlend);

			static std::unique_ptr<Param> generate_param();
	};

}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
