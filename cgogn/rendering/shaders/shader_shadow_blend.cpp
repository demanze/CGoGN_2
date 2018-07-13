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
#include <random>

#include <cgogn/rendering/shaders/shader_shadow_blend.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
		ShadowBlend* ShadowBlend::instance_ = nullptr;

		ShadowBlend::ShadowBlend()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "fullscreen_texture_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "shadow_blend_frag.glsl");
			
			link();

			bind();

			unif_sampler_scene_color = "sampler_scene_color";
			unif_sampler_shadow = "sampler_shadow";
			unif_sampler_scene_depth = "sampler_scene_depth";
			unif_sampler_scene_position = "sampler_scene_position";
			unif_sampler_scene_normal = "sampler_scene_normal";

			std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
			std::default_random_engine generator;
			std::vector<Vector3f> kernel;
			for (unsigned int i = 0; i < 64; ++i)
			{
				Vector3f sample(randomFloats(generator), randomFloats(generator), randomFloats(generator));
				sample = (sample / sample.norm()) * randomFloats(generator);
				kernel.push_back(sample);
			}
			unif_ssao_kernel = "ssao_kernel";
			unif_ssao_kernel.set(64, kernel.data());

			unif_projection_matrix = "projection_matrix";

			unif_enable_shadow = "enableShadow";
			unif_enable_ssao = "enableSSAO";

			release(); 
		}

		void ParamShadowBlend::set_enable_shadow(bool value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_enable_shadow.set(value);
		}

		void ParamShadowBlend::set_enable_ssao(bool value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_enable_ssao.set(value);
		}

		void ParamShadowBlend::set_sampler_scene_color(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_sampler_scene_color.set(value);
		}

		void ParamShadowBlend::set_sampler_shadow(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_sampler_shadow.set(value);
		}

		void ParamShadowBlend::set_sampler_scene_depth(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_sampler_scene_depth.set(value);
		}

		void ParamShadowBlend::set_sampler_scene_position(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_sampler_scene_position.set(value);
		}

		void ParamShadowBlend::set_sampler_scene_normal(GLint value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_sampler_scene_normal.set(value);
		}

		void ParamShadowBlend::set_projection_matrix(Matrix4f value)
		{
			ShadowBlend* sh = static_cast<ShadowBlend*>(this->program);
			sh->unif_projection_matrix.set(value);
		}

		std::unique_ptr< ShadowBlend::Param> ShadowBlend::generate_param()
		{
			if (!instance_)
			{
				instance_ = new ShadowBlend();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamShadowBlend::set_uniforms()
		{

		}

		ParamShadowBlend::ParamShadowBlend(ShadowBlend* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 