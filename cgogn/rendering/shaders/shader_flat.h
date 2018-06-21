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

#ifndef CGOGN_RENDERING_SHADERS_FLAT_H_
#define CGOGN_RENDERING_SHADERS_FLAT_H_

#include <cgogn/rendering/opengl/all.h>


namespace cgogn
{

namespace rendering
{

// forward
template <bool CPV>
class ShaderParamFlat: public ogl::ShaderParam
{};

class CGOGN_RENDERING_API ShaderFlatGen : public ogl::ShaderProgram
{
	template <bool CPV> friend class ShaderParamFlat;

protected:

	static const char* vertex_shader_source_;
	static const char* fragment_shader_source_;

	static const char* vertex_shader_source2_;
	static const char* fragment_shader_source2_;

	// uniform ids
	ogl::Uniform unif_front_color_;
	ogl::Uniform unif_back_color_;
	ogl::Uniform unif_ambiant_color_;
	ogl::Uniform unif_light_position_;
	ogl::Uniform unif_bf_culling_;

public:

	using Self = ShaderFlatGen;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderFlatGen);

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_COLOR
	};

	/**
	 * @brief set current front color
	 * @param rgb
	 */
	void set_front_color(const Color& rgb);

	/**
	 * @brief set current front color
	 * @param rgb
	 */
	void set_back_color(const Color& rgb);

	/**
	 * @brief set current ambiant color
	 * @param rgb
	 */
	void set_ambiant_color(const Color& rgb);

	/**
	 * @brief set light position relative to screen
	 * @param l light position
	 */
	void set_light_position(const Vector3f& l);

	/**
	 * @brief set light position relative to world
	 * @param l light position
	 * @param view_matrix
	 */
	void set_local_light_position(const Vector3f& l, const Matrix4f& view_matrix);

	void set_bf_culling(bool cull);

protected:

	ShaderFlatGen(bool color_per_vertex);
};

template <bool CPV>
class ShaderFlatTpl : public ShaderFlatGen
{
public:

	using Param = ShaderParamFlat<CPV>;
	static std::unique_ptr<Param> generate_param();

private:

	ShaderFlatTpl() : ShaderFlatGen(CPV) {}
	static ShaderFlatTpl* instance_;
};

template <bool CPV>
ShaderFlatTpl<CPV>* ShaderFlatTpl<CPV>::instance_ = nullptr;


// COLOR UNIFORM PARAM
template <>
class ShaderParamFlat<false> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderFlatGen* sh = static_cast<ShaderFlatGen*>(this->program);
		sh->set_front_color(front_color_);
		sh->set_back_color(back_color_);
		sh->set_ambiant_color(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_bf_culling(bf_culling_);
	}

public:

	using ShaderType = ShaderFlatTpl<false>;

	Color front_color_;
	Color back_color_;
	Color ambiant_color_;
	Vector3f light_pos_;
	bool bf_culling_;

	ShaderParamFlat(ShaderFlatTpl<false>* sh) :
		ogl::ShaderParam(sh),
		front_color_(Color(250, 0, 0)),
		back_color_(Color(0, 250, 0)),
		ambiant_color_(Color(5, 5, 5)),
		light_pos_(Vector3f(10, 100, 1000)),
		bf_culling_(false)
	{}

	void set_position_vbo(VBO* vbo_pos)
	{
		bind();
		vao_->bind();
		vao_->attribPointer(ShaderFlatGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		release();
	}
};

// COLOR PER VERTEX PARAM
template <>
class ShaderParamFlat<true> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderFlatGen* sh = static_cast<ShaderFlatGen*>(this->program);
		sh->set_ambiant_color(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_bf_culling(bf_culling_);
	}

public:

	using ShaderType = ShaderFlatTpl<true>;

	Color ambiant_color_;
	Vector3f light_pos_;
	bool bf_culling_;

	ShaderParamFlat(ShaderFlatTpl<true>* sh) :
		ogl::ShaderParam(sh),
		ambiant_color_(Color(5, 5, 5)),
		light_pos_(Vector3f(10, 100, 1000)),
		bf_culling_(false)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_color)
	{
		bind();
		vao_->bind();
		vao_->attribPointer(ShaderFlatGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderFlatGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		bind();
		vao_->bind();
		vao_->attribPointer(ShaderFlatGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		release();
	}

	void set_color_vbo(VBO* vbo_color)
	{
		bind();
		vao_->bind();
		vao_->attribPointer(ShaderFlatGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		release();
	}
};

template <bool CPV>
std::unique_ptr<typename ShaderFlatTpl<CPV>::Param> ShaderFlatTpl<CPV>::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderFlatTpl<CPV>();
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}

using ShaderFlat = ShaderFlatTpl<false>;
using ShaderFlatColor = ShaderFlatTpl<true>;

#if !defined(CGOGN_RENDER_SHADERS_FLAT_CPP_)
extern template class CGOGN_RENDERING_API ShaderFlatTpl<false>;
extern template class CGOGN_RENDERING_API ShaderFlatTpl<true>;
extern template class CGOGN_RENDERING_API ShaderParamFlat<false>;
extern template class CGOGN_RENDERING_API ShaderParamFlat<true>;
#endif

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_FLAT_H_
