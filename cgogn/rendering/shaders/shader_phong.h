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

#ifndef CGOGN_RENDERING_SHADERS_PHONG_H_
#define CGOGN_RENDERING_SHADERS_PHONG_H_

#include <cgogn/rendering/opengl/all.h>


namespace cgogn
{

namespace rendering
{

// forward
template <bool CPV>
class ShaderParamPhong: public ogl::ShaderParam
{};

class CGOGN_RENDERING_API ShaderPhongGen : public ogl::ShaderProgram
{
	template <bool CPV> friend class ShaderParamPhong;

protected:

	static const char* vertex_shader_source_;
	static const char* fragment_shader_source_;

	static const char* vertex_shader_source_2_;
	static const char* fragment_shader_source_2_;

	// uniform ids
	ogl::Uniform unif_front_color_;
	ogl::Uniform unif_back_color_;
	ogl::Uniform unif_ambiant_color_;
	ogl::Uniform unif_spec_color_;
	ogl::Uniform unif_spec_coef_;
	ogl::Uniform unif_double_side_;
	ogl::Uniform unif_light_position_;

public:

	using Self = ShaderPhongGen;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderPhongGen);

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_NORM,
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
	 * @brief set current specular color
	 * @param rgb
	 */
	void set_specular_color(const Color& rgb);

	/**
	 * @brief set current specular coefficient
	 * @param rgb
	 */
	void set_specular_coef(float32 coef);

	/**
	 * @brief set double side option
	 * @param ts
	 */
	void set_double_side(bool ts);

	/**
	 * @brief set_light_position
	 * @param l
	 */
	void set_light_position(const Vector3f& l);

	/**
	 * @brief set light position relative to world
	 * @param l light position
	 * @param view_matrix
	 */
	void set_local_light_position(const Vector3f& l, const Matrix4f& view_matrix);

protected:

	ShaderPhongGen(bool color_per_vertex);
};


template <bool CPV>
class ShaderPhongTpl : public ShaderPhongGen
{
public:

	using Param = ShaderParamPhong<CPV>;
	static std::unique_ptr<Param> generate_param();

private:

	ShaderPhongTpl() : ShaderPhongGen(CPV) {}
	static ShaderPhongTpl* instance_;
};

template <bool CPV>
ShaderPhongTpl<CPV>* ShaderPhongTpl<CPV>::instance_ = nullptr;


// COLOR UNIFORM PARAM
template <>
class ShaderParamPhong<false> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPhongGen* sh = static_cast<ShaderPhongGen*>(this->program);
		sh->set_front_color(front_color_);
		sh->set_back_color(back_color_);
		sh->set_ambiant_color(ambiant_color_);
		sh->set_specular_color(specular_color_);
		sh->set_specular_coef(specular_coef_);
		sh->set_double_side(double_side_);
		sh->set_light_position(light_position_);
	}

public:
	using ShaderType = ShaderPhongTpl<false>;

	Vector3f light_position_;
	Color front_color_;
	Color back_color_;
	Color ambiant_color_;
	Color specular_color_;
	float32 specular_coef_;
	bool double_side_;

	ShaderParamPhong(ShaderPhongTpl<false>* sh) :
		ogl::ShaderParam(sh),
		light_position_(0.0f, 0.0f, 1000.0f),
		front_color_(250, 0, 0),
		back_color_(0, 250, 5),
		ambiant_color_(5, 5, 5),
		specular_color_(100, 100, 100),
		specular_coef_(50.0f),
		double_side_(true)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_norm)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderPhongGen::ATTRIB_NORM, vbo_norm, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_normal_vbo(VBO* vbo_norm)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_NORM, vbo_norm, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

// COLOR PER VERTEX PARAM
template <>
class ShaderParamPhong<true> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPhongGen* sh = static_cast<ShaderPhongGen*>(this->program);
		sh->set_ambiant_color(ambiant_color_);
		sh->set_specular_color(specular_color_);
		sh->set_specular_coef(specular_coef_);
		sh->set_double_side(double_side_);
		sh->set_light_position(light_position_);
	}

public:
	using ShaderType = ShaderPhongTpl<true>;

	Vector3f light_position_;
	Color ambiant_color_;
	Color specular_color_;
	float32 specular_coef_;
	bool double_side_;

	ShaderParamPhong(ShaderPhongTpl<true>* sh) :
		ogl::ShaderParam(sh),
		light_position_(0.0f, 0.0f, 1000.0f),
		ambiant_color_(5, 5, 5),
		specular_color_(100, 100, 100),
		specular_coef_(50.0f),
		double_side_(true)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_norm, VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderPhongGen::ATTRIB_NORM, vbo_norm, GL_FLOAT);
		vao_->attribPointer(ShaderPhongGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{	
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_normal_vbo(VBO* vbo_norm)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_NORM, vbo_norm, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_color_vbo(VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPhongGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}
};


template <bool CPV>
std::unique_ptr<typename ShaderPhongTpl<CPV>::Param> ShaderPhongTpl<CPV>::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderPhongTpl<CPV>;
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}


using ShaderPhong = ShaderPhongTpl<false>;
using ShaderPhongColor = ShaderPhongTpl<true>;


#if !defined(CGOGN_RENDER_SHADERS_PHONG_CPP_)
extern template class CGOGN_RENDERING_API ShaderPhongTpl<false>;
extern template class CGOGN_RENDERING_API ShaderPhongTpl<true>;
extern template class CGOGN_RENDERING_API ShaderParamPhong<false>;
extern template class CGOGN_RENDERING_API ShaderParamPhong<true>;
#endif
} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_PHONG_H_
