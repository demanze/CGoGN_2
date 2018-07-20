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

#include <cgogn/rendering/shaders/shader_shadow.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
	namespace Shadow
	{
		Shader* Shader::instance_ = nullptr;

		Shader::Shader()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "shadow_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "shadow_frag.glsl");

			bindAttributeLocation("vertex_pos", ATTRIB_POS);
			bindAttributeLocation("vertex_norm", ATTRIB_NORM);

			link();

			bind();

			unif_shadowMap_ = "shadowMap";
			unif_shadowMVP_ = "shadowMVP";
			unif_pixelSize_ = "pixelSize";

			unif_sampler_scene_position = "sampler_scene_position";
			unif_sampler_scene_normal = "sampler_scene_normal";
			unif_sampler_noise = "sampler_noise";

			std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
			std::default_random_engine generator;
			std::vector<Vector3f> kernel;

			for (unsigned int i = 0; i < 64; ++i)
			{
				Vector3f sample(randomFloats(generator)* 2.0 - 1.0, randomFloats(generator)* 2.0 - 1.0, randomFloats(generator));
				sample = (sample / sample.norm()) * randomFloats(generator);
				kernel.push_back(sample);
			}

			unif_ssao_kernel = "ssao_kernel";
			unif_ssao_kernel.set(256, kernel.data());
			unif_noise_scale = "noiseScale";

			unif_enable_shadow = "enableShadow";
			unif_enable_ssao = "enableSSAO";
			unif_enable_border = "enableBorder";

			unif_radius_ssao = "radiusSSAO";
			unif_radius_border = "radiusBorder";

			get_matrices_uniforms();

			release();
		}

		void Param::set_shadowMap(GLint value)
		{
			shader()->unif_shadowMap_.set(value);
		}

		void Param::set_shadowMVP(float* value)
		{
			shader()->unif_shadowMVP_.set(Matrix4f(value));
		}

		void Param::set_pixelSize(float value)
		{
			shader()->unif_pixelSize_.set(value);
		}

		void Param::set_enable_shadow(bool value)
		{
			shader()->unif_enable_shadow.set(value);
		}

		void Param::set_enable_ssao(bool value)
		{
			shader()->unif_enable_ssao.set(value);
		}

		void Param::set_enable_border(bool value)
		{
			shader()->unif_enable_border.set(value);
		}

		void Param::set_radius_ssao(float value)
		{
			shader()->unif_radius_ssao.set(value);
		}

		void Param::set_radius_border(float value)
		{;
			shader()->unif_radius_border.set(value);
		}

		void Param::set_sampler_scene_position(GLint value)
		{
			shader()->unif_sampler_scene_position.set(value);
		}

		void Param::set_sampler_scene_normal(GLint value)
		{
			shader()->unif_sampler_scene_normal.set(value);
		}

		void Param::set_sampler_noise(GLint value)
		{
			shader()->unif_sampler_noise.set(value);
		}

		void Param::set_noise_scale(Vector2f value)
		{
			shader()->unif_noise_scale.set(value);
		}

		void Param::set_uniforms()
		{

		}

		std::unique_ptr<Param> Param::generate()
		{
			if (!Shader::instance_)
			{
				Shader::instance_ = new Shader();
				Shader::ShaderProgram::register_instance(Shader::instance_);
			}
			return cgogn::make_unique<Param>(Shader::instance_);
		}
	}

}

} 

} 