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

#include <cgogn/rendering/shaders/shader_postprocessing_1.h>


namespace cgogn
{

namespace rendering
{

namespace shaders2
{

		const char* PostProcessing1::vertex_shader_source_ =
			"#version 330\n"
			"out vec2 uv;\n"
			"void main()\n"
			"{\n"
			"	if (gl_VertexID == 0) {\n"
			"	gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(0.0f,0.0f);\n"
			"   } else if (gl_VertexID == 1) {\n"
			"	gl_Position = vec4(1.0f, -1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(1.0f,0.0f);\n"
			"   } else if (gl_VertexID == 2) {\n"
			"	gl_Position = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(1.0f,1.0f);\n"
			"   } else if (gl_VertexID == 3) {\n"
			"	gl_Position = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(1.0f,1.0f);\n"
			"   } else if (gl_VertexID == 4) {\n"
			"	gl_Position = vec4(-1.0f, 1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(0.0f,1.0f);\n"
			"   } else if (gl_VertexID == 5) {\n"
			"	gl_Position = vec4(-1.0f, -1.0f, 0.0f, 1.0f);\n"
			"	uv = vec2(0.0f,0.0f);\n"
			"   }\n"
			"}\n";

		const char* PostProcessing1::fragment_shader_source_ =
			"#version 330\n"
			"layout(location = 0) out vec4 color_out;\n"
			"in vec2 uv;\n"
			"const float kernel[11] = float[11](0.084264, 0.088139, 0.091276, 0.093585, 0.094998, 0.095474, 0.094998, 0.093585, 0.091276, 0.088139, 0.084264);\n"
			"const int kernelSize = 11;\n"
			"uniform sampler2D rgba_texture;\n"
			"uniform uint dimension;\n"
			"uniform float pixel_size;\n"
			"void main()\n"
			"{\n"
			"	vec3 sum = vec3(0.0f,0.0f,0.0f);\n"
			"	for (int i=0;i<kernelSize;i++) {\n"
			"	vec2 offset = uv;\n"
			"	offset[dimension] += pixel_size * float(i - (kernelSize / 2));\n"
			"   sum += texture(rgba_texture, offset).rgb*kernel[i];\n"
			"	}\n"
			"	color_out = vec4(sum,1.0f);"
			"}\n";

		PostProcessing1* PostProcessing1::instance_ = nullptr;

		PostProcessing1::PostProcessing1()
		{
			addShader(GL_VERTEX_SHADER, vertex_shader_source_);
			addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
			link();

			bind();

			unif_rgba_texture_sampler = "rgba_texture";
			unif_blur_dimension = "dimension"; 
			unif_pixel_size = "pixel_size"; 

			release(); 
		}

		void ParamPostProcessing1::set_rgba_sampler(GLint value)
		{
			PostProcessing1* sh = static_cast<PostProcessing1*>(this->program);
			sh->unif_rgba_texture_sampler.set(value);
		}

		void ParamPostProcessing1::set_blur_dimension(GLuint value)
		{
			PostProcessing1* sh = static_cast<PostProcessing1*>(this->program);
			sh->unif_blur_dimension.set(value);  
		}

		void ParamPostProcessing1::set_pixel_size(float32 value)
		{
			PostProcessing1* sh = static_cast<PostProcessing1*>(this->program);
			sh->unif_pixel_size.set(value);
		}

		std::unique_ptr< PostProcessing1::Param> PostProcessing1::generate_param()
		{
			if (!instance_)
			{
				instance_ = new PostProcessing1();
				ShaderProgram::register_instance(instance_);
			}
			return cgogn::make_unique<Param>(instance_);
		}

		void ParamPostProcessing1::set_uniforms()
		{

		}

		ParamPostProcessing1::ParamPostProcessing1(PostProcessing1* sh) :
			ogl::ShaderParam(sh)
		{
		}
	}

} 

} 