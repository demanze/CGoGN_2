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

#ifndef CGOGN_RENDERING_SHADERS_TEXT_H_
#define CGOGN_RENDERING_SHADERS_TEXT_H_

#include <cgogn/rendering/opengl/all.h>

namespace cgogn
{

namespace rendering
{

class ShaderText;

class CGOGN_RENDERING_API ShaderParamText : public ogl::ShaderParam
{
protected:

	void set_uniforms();

public:
	using ShaderType = ShaderText;

	std::unique_ptr<ogl::Texture>* texture_;

	float32 italic_;

	ShaderParamText(ShaderText* sh);

	void set_vbo(VBO* vbo_pos, VBO* vbo_str, VBO* vbo_colsize);
};

class CGOGN_RENDERING_API ShaderText : public ogl::ShaderProgram
{
	static const char* vertex_shader_source_;
	static const char* fragment_shader_source_;
	ogl::Uniform unif_italic_;

public:

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_CHAR,
		ATTRIB_COLSZ
	};

	using Param = ShaderParamText;

	/**
	 * @brief generate shader parameter object
	 * @return pointer
	 */
	static std::unique_ptr<Param> generate_param();

	/**
	 * @brief set_italic
	 * @param i %
	 */
	void set_italic(float32 i);

protected:
	ShaderText();
	static ShaderText* instance_;
};

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_TEXTURE_H_
