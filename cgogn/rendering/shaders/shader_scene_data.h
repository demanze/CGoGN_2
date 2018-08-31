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

#ifndef CGOGN_RENDERING_SHADER_SCENEDATA_H_
#define CGOGN_RENDERING_SHADER_SCENEDATA_H_

#include <cgogn/rendering/opengl/all.h>

namespace cgogn
{

namespace rendering
{

namespace shaders
{

	namespace SceneData
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

		};


		class CGOGN_RENDERING_API Param : public ogl::ShaderParam
		{
		public:
			Param(Shader* sh) : ShaderParam(sh) {}
			auto shader() { return static_cast<Shader*>(this->program); };

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

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_