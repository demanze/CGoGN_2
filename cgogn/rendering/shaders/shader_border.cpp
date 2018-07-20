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

#include <cgogn/rendering/shaders/shader_border.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{
	namespace Border
	{
		Shader* Shader::instance_ = nullptr;

		Shader::Shader()
		{
			addShaderFromFile(GL_VERTEX_SHADER, "border_vert.glsl");
			addShaderFromFile(GL_FRAGMENT_SHADER, "border_frag.glsl");

			bindAttributeLocation("vertex_pos", ATTRIB_POS);

			link();

			bind();

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
			unif_ssao_kernel.set(64, kernel.data());
			unif_noise_scale = "noiseScale";

			unif_radius = "radius";

			get_matrices_uniforms();

			release();
		}

		void Param::set_radius(float value)
		{
			shader()->unif_radius.set(value);
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