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

#ifndef CGOGN_RENDERING_SHADERS_BOLDLINE_H_
#define CGOGN_RENDERING_SHADERS_BOLDLINE_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/shader_program.h>
#include <cgogn/rendering/shaders/vbo.h>

namespace cgogn
{

namespace rendering
{

// forward
template <bool CPV>
class ShaderParamBoldLine : public ogl::ShaderParam
{};

class CGOGN_RENDERING_API ShaderBoldLineGen : public ogl::ShaderProgram
{
	template <bool CPV> friend class ShaderParamBoldLine;

protected:

	static const char* vertex_shader_source_;
	static const char* geometry_shader_source_;
	static const char* fragment_shader_source_;

	static const char* vertex_shader_source2_;
	static const char* geometry_shader_source2_;
	static const char* fragment_shader_source2_;

	// uniform ids
	ogl::Uniform unif_color_;
	ogl::Uniform unif_width_;
	ogl::Uniform unif_plane_clip_;
	ogl::Uniform unif_plane_clip2_;

public:

	using Self = ShaderBoldLineGen;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderBoldLineGen);

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_COLOR
	};

	/**
	 * @brief set current color
	 * @param rgb
	 */
	void set_color(const Color& rgb);

	/**
	 * @brief set the width of lines (call before each draw)
	 * @param w width in pixel
	 */
	void set_width(float32 w);

	/**
	 * @brief set_plane_clip
	 * @param plane
	 */
	void set_plane_clip(const Vector4f& plane);

	/**
	 * @brief set_plane_clip2
	 * @param plane
	 */
	void set_plane_clip2(const Vector4f& plane);


protected:

	ShaderBoldLineGen(bool color_per_vertex);
};

template <bool CPV>
class ShaderBoldLineTpl : public ShaderBoldLineGen
{
public:

	using Param = ShaderParamBoldLine<CPV>;
	static std::unique_ptr<Param> generate_param();

private:

	ShaderBoldLineTpl() : ShaderBoldLineGen(CPV) {}
	static ShaderBoldLineTpl* instance_;
};

template <bool CPV>
ShaderBoldLineTpl<CPV>* ShaderBoldLineTpl<CPV>::instance_ = nullptr;


// COLOR UNIFORM VERSION
template <>
class ShaderParamBoldLine<false> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderBoldLineGen* sh = static_cast<ShaderBoldLineGen*>(this->program);
		sh->set_color(color_);
		sh->set_width(width_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:
	using ShaderType = ShaderBoldLineTpl<false>;

	Color color_;
	float32 width_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;

	ShaderParamBoldLine(ShaderBoldLineTpl<false>* sh) :
		ogl::ShaderParam(sh),
		color_(Color(255, 255, 255)),
		width_(2.0f),
		plane_clip_(Vector4f(0,0,0,0)),
		plane_clip2_(Vector4f(0,0,0,0))
	{}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderBoldLineGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

// COLOR PER VERTEX VERSION
template <>
class ShaderParamBoldLine<true> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderBoldLineGen* sh = static_cast<ShaderBoldLineGen*>(this->program);
		sh->set_width(width_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderBoldLineTpl<true>;
	using Self = ShaderParamBoldLine<true>;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderParamBoldLine);

	float32 width_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;

	ShaderParamBoldLine(ShaderBoldLineTpl<true>* sh) :
		ogl::ShaderParam(sh),
		width_(2.0f),
		plane_clip_(0,0,0,0),
		plane_clip2_(0,0,0,0)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderBoldLineGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderBoldLineGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderBoldLineGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_color_vbo(VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderBoldLineGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}
};


template <bool CPV>
std::unique_ptr<typename ShaderBoldLineTpl<CPV>::Param> ShaderBoldLineTpl<CPV>::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderBoldLineTpl<CPV>;
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}


using ShaderBoldLine = ShaderBoldLineTpl<false>;
using ShaderBoldLineColor = ShaderBoldLineTpl<true>;


#if !defined(CGOGN_RENDER_SHADERS_BOLD_LINE_CPP_)
extern template class CGOGN_RENDERING_API ShaderBoldLineTpl<false>;
extern template class CGOGN_RENDERING_API ShaderBoldLineTpl<true>;
extern template class CGOGN_RENDERING_API ShaderParamBoldLine<false>;
extern template class CGOGN_RENDERING_API ShaderParamBoldLine<true>;
#endif

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_BOLDLINE_H_
