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
			namespace Shadow
			{
				class CGOGN_RENDERING_API Shader : public ogl::ShaderProgram
				{
					friend class Param;

					enum
					{
						ATTRIB_POS = 0,
						ATTRIB_NORM = 1,
					};

				private:
					static Shader* instance_;
					Shader();

				protected:

					ogl::Uniform unif_shadowMap_;
					ogl::Uniform unif_shadowMVP_;
					ogl::Uniform unif_pixelSize_;

					ogl::Uniform unif_sampler_scene_position;
					ogl::Uniform unif_sampler_scene_normal;
					ogl::Uniform unif_sampler_noise;

					ogl::Uniform unif_ssao_kernel;
					ogl::Uniform unif_noise_scale;

					ogl::Uniform unif_enable_shadow;
					ogl::Uniform unif_enable_ssao;
					ogl::Uniform unif_enable_border;

					ogl::Uniform unif_radius_ssao;
					ogl::Uniform unif_radius_border;
				};


				class CGOGN_RENDERING_API Param : public ogl::ShaderParam
				{
				public:
					Param(Shader* sh) : ShaderParam(sh) {}
					auto shader() { return static_cast<Shader*>(this->program); };

					void set_shadowMap(GLint value);
					void set_shadowMVP(float* value);
					void set_pixelSize(float value);
					void set_enable_shadow(bool value);
					void set_enable_ssao(bool value);
					void set_enable_border(bool value);
					void set_radius_ssao(float value);
					void set_radius_border(float value);
					void set_sampler_scene_position(GLint value);
					void set_sampler_scene_normal(GLint value);
					void set_sampler_noise(GLint value);
					void set_noise_scale(Vector2f value);
					void set_uniforms();

					void set_vbos(VBO* vbo_pos, VBO* vbo_norm)
					{
						bind();
						vao_->bind();
						vao_->attribPointer(Shader::ATTRIB_POS, vbo_pos, GL_FLOAT);
						vao_->attribPointer(Shader::ATTRIB_NORM, vbo_norm, GL_FLOAT);
						vao_->release();
						release();
					}

					static std::unique_ptr<Param> generate();
				};
			}
}

} 

} 
#endif 
