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
	class ParamShadow;

	class CGOGN_RENDERING_API Shadow : public ogl::ShaderProgram
	{
		friend class ParamShadow;

		enum
		{
			ATTRIB_POS = 0
		};

	private:
		static Shadow* instance_;
		Shadow();

		ogl::Uniform unif_shadowMap_;
		ogl::Uniform unif_shadowMVP_;

	public:
		using Param = ParamShadow;
		using Self = Shadow;
		CGOGN_NOT_COPYABLE_NOR_MOVABLE(Shadow);

		static std::unique_ptr<Param> generate_param();
	};


	class CGOGN_RENDERING_API ParamShadow : public ogl::ShaderParam
	{
		public:
			using Type = Shadow;

			ParamShadow(Shadow* sh);

			void set_shadowMap(GLint value);
			void set_shadowMVP(float* value);

			void set_uniforms(); 

			void set_position_vbo(VBO* vbo_pos)
			{
				bind();
				vao_->bind();
				vao_->attribPointer(Shadow::ATTRIB_POS, vbo_pos, GL_FLOAT);
				vao_->release();
				release();
			}
	};
}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
