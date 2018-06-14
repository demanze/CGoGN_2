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

#ifndef CGOGN_RENDERING_SHADER_POINT_SPRITE_H_
#define CGOGN_RENDERING_SHADER_POINT_SPRITE_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/shader_program.h>
#include <cgogn/rendering/shaders/vbo.h>

#include <type_traits>

namespace cgogn
{

namespace rendering
{

// forward
template <bool CPV, bool SPV>
class ShaderParamPointSprite : public ogl::ShaderParam
{};

class CGOGN_RENDERING_API ShaderPointSpriteGen : public ogl::ShaderProgram
{
	template <bool CPV, bool SPV> friend class ShaderParamPointSprite;

protected:

	static const char* vertex_shader_source_;
	static const char* geometry_shader_source_;
	static const char* fragment_shader_source_;

	// uniform ids
	ogl::Uniform unif_color_;
	ogl::Uniform unif_size_;
	ogl::Uniform unif_ambiant_;
	ogl::Uniform unif_light_pos_;
	ogl::Uniform unif_plane_clip_;
	ogl::Uniform unif_plane_clip2_;

public:

	using Self = ShaderPointSpriteGen;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderPointSpriteGen);

	enum
	{
		ATTRIB_POS = 0,
		ATTRIB_COLOR,
		ATTRIB_SIZE
	};

	void set_color(const Color& rgb);

	/**
	* @brief set ambiant color
	* @param rgb
	*/
	void set_ambiant(const Color& rgb);

	/**
	* @brief set light position relative to screen
	* @param l
	*/
	void set_light_position(const Vector3f& l);

	/**
	* @brief set light position relative to world
	* @param l
	* @param view_matrix
	*/
	void set_local_light_position(const Vector3f& l, const Matrix4f& view_matrix);

	/**
	* @brief set the size of sphere (call before each draw)
	* @param w size ofs phere
	*/
	void set_size(float32 w);

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

	ShaderPointSpriteGen(bool color_per_vertex, bool size_per_vertex);
};

template <bool CPV, bool SPV>
class ShaderPointSpriteTpl : public ShaderPointSpriteGen
{
public:

	using Param = ShaderParamPointSprite<CPV, SPV>;
	static std::unique_ptr<Param> generate_param();

private:

	ShaderPointSpriteTpl() : ShaderPointSpriteGen(CPV, SPV) {}
	static ShaderPointSpriteTpl* instance_;
};


template <bool CPV, bool SPV>
ShaderPointSpriteTpl<CPV,SPV>* ShaderPointSpriteTpl<CPV, SPV>::instance_ = nullptr;


template <>
class ShaderParamPointSprite<false, false> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPointSpriteGen* sh = static_cast<ShaderPointSpriteGen*>(this->program);
		sh->set_color(color_);
		sh->set_size(size_);
		sh->set_ambiant(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderPointSpriteTpl<false, false>;

	Color color_;
	Color ambiant_color_;
	Vector3f light_pos_;
	float32 size_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;

	ShaderParamPointSprite(ShaderPointSpriteTpl<false,false>* sh) :
		ogl::ShaderParam(sh),
		color_(Color(0, 0, 255)),
		ambiant_color_(Color(5, 5, 5)),
		light_pos_(Vector3f(10, 100, 1000)),
		size_(1.0),
		plane_clip_(Vector4f(0,0,0,0)),
		plane_clip2_(Vector4f(0,0,0,0))
	{}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

template <>
class ShaderParamPointSprite<false, true> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPointSpriteGen* sh = static_cast<ShaderPointSpriteGen*>(this->program);
		sh->set_color(color_);
		sh->set_ambiant(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderPointSpriteTpl<false, true>;

	Color color_;
	Color ambiant_color_;
	Vector3f light_pos_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;


	ShaderParamPointSprite(ShaderPointSpriteTpl<false, true>* sh) :
		ogl::ShaderParam(sh),
		color_(Color(0, 0, 255)),
		ambiant_color_(Color(5, 5, 5)),
		light_pos_(Vector3f(10, 100, 1000)),
		plane_clip_(Vector4f(0,0,0,0)),
		plane_clip2_(Vector4f(0,0,0,0))
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_size)
	{
		
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_SIZE, vbo_size, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_size_vbo(VBO* vbo_size)
	{
		program->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_SIZE, vbo_size, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

template <>
class ShaderParamPointSprite<true, false> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPointSpriteGen* sh = static_cast<ShaderPointSpriteGen*>(this->program);
		sh->set_ambiant(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_size(size_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderPointSpriteTpl<true, false>;

	Color ambiant_color_;
	Vector3f light_pos_;
	float32 size_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;


	ShaderParamPointSprite(ShaderPointSpriteTpl<true, false>* sh) :
		ogl::ShaderParam(sh),
		ambiant_color_(5, 5, 5),
		light_pos_(10, 100, 1000),
		size_(1.0),
		plane_clip_(0,0,0,0),
		plane_clip2_(0,0,0,0)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_color_vbo(VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}
};

template <>
class ShaderParamPointSprite<true, true> : public ogl::ShaderParam
{
protected:

	void set_uniforms() override
	{
		ShaderPointSpriteGen* sh = static_cast<ShaderPointSpriteGen*>(this->program);
		sh->set_ambiant(ambiant_color_);
		sh->set_light_position(light_pos_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderPointSpriteTpl<true, true>;

	Color ambiant_color_;
	Vector3f light_pos_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;


	ShaderParamPointSprite(ShaderPointSpriteTpl<true, true>* sh) :
		ogl::ShaderParam(sh),
		ambiant_color_(Color(5, 5, 5)),
		light_pos_(Vector3f(10, 100, 1000)),
		plane_clip_(0,0,0,0),
		plane_clip2_(0,0,0,0)
	{}

	void set_all_vbos(VBO* vbo_pos, VBO* vbo_color, VBO* vbo_size)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_SIZE, vbo_size, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_color_vbo(VBO* vbo_color)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_COLOR, vbo_color, GL_FLOAT);
		vao_->release();
		program->release();
	}

	void set_size_vbo(VBO* vbo_size)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderPointSpriteGen::ATTRIB_SIZE, vbo_size, GL_FLOAT);
		vao_->release();
		program->release();
	}
};


template <bool CPV, bool SPV>
std::unique_ptr<typename ShaderPointSpriteTpl<CPV, SPV>::Param> ShaderPointSpriteTpl<CPV, SPV>::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderPointSpriteTpl<CPV, SPV>;
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}


using ShaderPointSprite = ShaderPointSpriteTpl<false, false>;
using ShaderPointSpriteColor = ShaderPointSpriteTpl<true, false>;
using ShaderPointSpriteSize = ShaderPointSpriteTpl<false, true>;
using ShaderPointSpriteColorSize = ShaderPointSpriteTpl<true, true>;


#if !defined(CGOGN_RENDER_SHADERS_POINT_SPRITE_CPP_)
extern template class CGOGN_RENDERING_API  ShaderPointSpriteTpl<false, false>;
extern template class CGOGN_RENDERING_API ShaderPointSpriteTpl<true, false>;
extern template class CGOGN_RENDERING_API ShaderPointSpriteTpl<false, true>;
extern template class CGOGN_RENDERING_API ShaderPointSpriteTpl<true, true>;
extern template class CGOGN_RENDERING_API ShaderParamPointSprite<false, false>;
extern template class CGOGN_RENDERING_API ShaderParamPointSprite<true, false>;
extern template class CGOGN_RENDERING_API ShaderParamPointSprite<false, true>;
extern template class CGOGN_RENDERING_API ShaderParamPointSprite<true, true>;
#endif

} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_POINT_SPRITE_H_
