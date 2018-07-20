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

	// forward
	class ParamSceneData;

	class CGOGN_RENDERING_API SceneData : public ogl::ShaderProgram
	{
		friend class ParamSceneData;

		enum
		{
			ATTRIB_POS = 0,
			ATTRIB_NORM = 1,
		};

	private:
		static SceneData* instance_;
		SceneData();

	public:
		using Param = ParamSceneData;
		using Self = SceneData;
		CGOGN_NOT_COPYABLE_NOR_MOVABLE(SceneData);

		static std::unique_ptr<Param> generate_param();
	};


	class CGOGN_RENDERING_API ParamSceneData : public ogl::ShaderParam
	{
		public:
			using Type = SceneData;

			ParamSceneData(SceneData* sh);

			void set_uniforms(); 

			void set_position_vbo(VBO* vbo_pos, VBO* vbo_norm)
			{
				bind();
				vao_->bind();
				vao_->attribPointer(SceneData::ATTRIB_POS, vbo_pos, GL_FLOAT);
				vao_->attribPointer(SceneData::ATTRIB_NORM, vbo_norm, GL_FLOAT);
				vao_->release();
				release();
			}
	};
}

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_FLAT_H_
