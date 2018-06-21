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

#ifndef CGOGN_RENDERING_SHADERS_VECTORPERVERTEX_H_
#define CGOGN_RENDERING_SHADERS_VECTORPERVERTEX_H_

#include <cgogn/rendering/opengl/all.h>


namespace cgogn
{

namespace rendering
{

class ShaderParamVectorPerVertex;

class CGOGN_RENDERING_API ShaderVectorPerVertex : public ogl::ShaderProgram
{
	friend class ShaderParamVectorPerVertex;

	static const char* vertex_shader_source_;
	static const char* geometry_shader_source_;
	static const char* fragment_shader_source_;

	// uniform ids
	ogl::Uniform unif_color_;
	ogl::Uniform unif_length_;

public:

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_NORMAL
	};

	using Param = ShaderParamVectorPerVertex;
	static std::unique_ptr<Param> generate_param();

	/**
	 * @brief set current color
	 * @param rgb
	 */
	void set_color(const Color& rgb);

	/**
	 * @brief set length of normal
	 * @param l length
	 */
	void set_length(float32 l);

protected:

	ShaderVectorPerVertex();
	static ShaderVectorPerVertex* instance_;
};

class CGOGN_RENDERING_API ShaderParamVectorPerVertex : public ogl::ShaderParam
{
protected:

	inline void set_uniforms() override
	{
		ShaderVectorPerVertex* sh = static_cast<ShaderVectorPerVertex*>(this->program);
		sh->set_color(color_);
		sh->set_length(length_);
	}

public:

	using ShaderType = ShaderVectorPerVertex;

	Color color_;
	float32 length_;

	ShaderParamVectorPerVertex(ShaderVectorPerVertex* sh) :
		ogl::ShaderParam(sh),
		color_(Color(255, 255, 255)),
		length_(1.0)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_vect)
	{
		
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderVectorPerVertex::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderVectorPerVertex::ATTRIB_NORMAL, vbo_vect, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderVectorPerVertex::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_vector_vbo(VBO* vbo_vect)
	{
		
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderVectorPerVertex::ATTRIB_NORMAL, vbo_vect, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_VECTORPERVERTEX_H_
