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

#ifndef CGOGN_RENDERING_SHADERS_EXPLODE_VOLUMES_LINE_H_
#define CGOGN_RENDERING_SHADERS_EXPLODE_VOLUMES_LINE_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/shader_program.h>
#include <cgogn/rendering/shaders/vbo.h>

namespace cgogn
{

namespace rendering
{

// forward
class ShaderParamExplodeVolumesLine;

class CGOGN_RENDERING_API ShaderExplodeVolumesLine : public ogl::ShaderProgram
{
	friend class ShaderParamExplodeVolumesLine;

protected:

	static const char* vertex_shader_source_;
	static const char* geometry_shader_source_;
	static const char* fragment_shader_source_;

	// uniform ids
	ogl::Uniform unif_expl_v_;
	ogl::Uniform unif_plane_clip_;
	ogl::Uniform unif_plane_clip2_;
	ogl::Uniform unif_color_;

public:

	enum
	{
		ATTRIB_POS = 0,
	};

	using Param = ShaderParamExplodeVolumesLine;
	static std::unique_ptr<Param> generate_param();

	void set_explode_volume(float32 x);
	void set_plane_clip(const Vector4f& plane);
	void set_plane_clip2(const Vector4f& plane);
	void set_color(const Vector4f& rgb);

protected:

	ShaderExplodeVolumesLine();
	static ShaderExplodeVolumesLine* instance_;
};

class CGOGN_RENDERING_API ShaderParamExplodeVolumesLine : public ogl::ShaderParam
{
protected:

	inline void set_uniforms() override
	{
		ShaderExplodeVolumesLine* sh = static_cast<ShaderExplodeVolumesLine*>(this->program);
		sh->set_color(color_);
		sh->set_explode_volume(explode_factor_);
		sh->set_plane_clip(plane_clip_);
		sh->set_plane_clip2(plane_clip2_);
	}

public:

	using ShaderType = ShaderExplodeVolumesLine;

	Vector4f color_;
	Vector4f plane_clip_;
	Vector4f plane_clip2_;
	float32 explode_factor_;

	ShaderParamExplodeVolumesLine(ShaderExplodeVolumesLine* sh) :
		ogl::ShaderParam(sh),
		color_(Color(255, 255, 255)),
		plane_clip_(Vector4f(0, 0, 0, 0)),
		plane_clip2_(Vector4f(0, 0, 0, 0)),
		explode_factor_(0.8f)
	{}

	void set_position_vbo(VBO* vbo_pos)
	{
		program->bind();
		vao_->bind();
		vao_->attribPointer(ShaderExplodeVolumesLine::ATTRIB_POS, vbo_pos, GL_FLOAT);
		vao_->release();
		program->release();
	}
};


} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADERS_EXPLODE_VOLUMES_LINE_H_
