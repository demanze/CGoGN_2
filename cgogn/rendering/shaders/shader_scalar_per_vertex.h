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

#ifndef CGOGN_RENDERING_SHADERS_SCALARPERVERTEX_H_
#define CGOGN_RENDERING_SHADERS_SCALARPERVERTEX_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/shader_program.h>
#include <cgogn/rendering/shaders/vbo.h>

namespace cgogn
{

namespace rendering
{

// forward
class ShaderParamScalarPerVertex;

class CGOGN_RENDERING_API ShaderScalarPerVertex : public ogl::ShaderProgram
{
	friend class ShaderParamScalarPerVertex;

protected:

	static const char* vertex_shader_source_;
	static const char* fragment_shader_source_;

	// uniform ids
	ogl::Uniform unif_color_map_;
	ogl::Uniform unif_expansion_;
	ogl::Uniform unif_min_value_;
	ogl::Uniform unif_max_value_;
	ogl::Uniform unif_show_iso_lines_;
	ogl::Uniform unif_nb_iso_levels_;

public:

	using Self = ShaderScalarPerVertex;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderScalarPerVertex);

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_SCALAR
	};

	enum ColorMap
	{
		BWR = 0,
		CWR,
		BCGYR,
		BGR
	};

	using Param = ShaderParamScalarPerVertex;
	static std::unique_ptr<Param> generate_param();

	/**
	 * @brief set current color map
	 * @param cm
	 */
	void set_color_map(ColorMap cm);

	/**
	 * @brief set current expansion factor
	 * @param expansion
	 */
	void set_expansion(int32 expansion);

	/**
	 * @brief set current scalar attribute minimum value
	 * @param value
	 */
	void set_min_value(float32 value);

	/**
	 * @brief set current scalar attribute maximum value
	 * @param value
	 */
	void set_max_value(float32 value);

	/**
	 * @brief set current show_iso_lines value
	 * @param b
	 */
	void set_show_iso_lines(bool b);

	/**
	 * @brief set current nb iso levels
	 * @param nb
	 */
	void set_nb_iso_levels(int32 nb);

protected:

	ShaderScalarPerVertex();
	static ShaderScalarPerVertex* instance_;
};

class CGOGN_RENDERING_API ShaderParamScalarPerVertex : public ogl::ShaderParam
{
protected:

	inline void set_uniforms() override
	{
		ShaderScalarPerVertex* sh = static_cast<ShaderScalarPerVertex*>(this->program);
		sh->set_color_map(color_map_);
		sh->set_expansion(expansion_);
		sh->set_min_value(min_value_);
		sh->set_max_value(max_value_);
		sh->set_show_iso_lines(show_iso_lines_);
		sh->set_nb_iso_levels(nb_iso_levels_);
	}

public:

	using ShaderType = ShaderScalarPerVertex;

	ShaderScalarPerVertex::ColorMap color_map_;
	int32 expansion_;
	float32 min_value_;
	float32 max_value_;
	bool show_iso_lines_;
	int32 nb_iso_levels_;

	ShaderParamScalarPerVertex(ShaderScalarPerVertex* prg) :
		ShaderParam(prg),
		color_map_(ShaderScalarPerVertex::BWR),
		expansion_(0),
		min_value_(.0f),
		max_value_(1.0f),
		show_iso_lines_(false),
		nb_iso_levels_(10)
	{}

	/**
	 * @brief set a vbo configuration
	 * @param vbo_pos pointer on position vbo (XYZ)
	 * @param vbo_col pointer on color vbo (RGB)
	 */
	void set_all_vbos(VBO* vbo_pos, VBO* vbo_scalar)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderScalarPerVertex::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderScalarPerVertex::ATTRIB_SCALAR, vbo_scalar, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderScalarPerVertex::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_scalar_vbo(VBO* vbo_scalar)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderScalarPerVertex::ATTRIB_SCALAR, vbo_scalar, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_SCALARPERVERTEX_H_
