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

#ifndef CGOGN_RENDERING_SHADER_TRANSP_PHONG_H_
#define CGOGN_RENDERING_SHADER_TRANSP_PHONG_H_

#include <cgogn/rendering/opengl/all.h>
#include <cgogn/rendering/dll.h>
#include <cgogn/rendering/shaders/shader_program.h>
#include <cgogn/rendering/shaders/vbo.h>

namespace cgogn
{

namespace rendering
{

// forward
class ShaderPhongTransp;

class CGOGN_RENDERING_API ShaderParamPhongTransp : public ogl::ShaderParam
{
protected:

	void set_uniforms() override;

public:
	using ShaderType = ShaderPhongTransp;

	Vector4f front_color_;
	Vector4f back_color_;
	Vector4f ambiant_color_;
	Vector4f specular_color_;;
	GLfloat specular_coef_;
	Vector3f light_pos_;
	bool bf_culling_;

	ShaderParamPhongTransp(ShaderPhongTransp* sh);

	void set_position_vbo(VBO* vbo_pos);

	void set_normal_vbo(VBO* vbo_normal);

	void set_alpha(int alpha);

};




class CGOGN_RENDERING_API ShaderPhongTransp : public ogl::ShaderProgram
{
	friend class ShaderParamPhongTransp;

protected:

	static const char* vertex_shader_source_;
	static const char* fragment_shader_source_;

	// uniform ids
	ogl::Uniform unif_front_color_;
	ogl::Uniform unif_back_color_;
	ogl::Uniform unif_ambiant_color_;
	ogl::Uniform unif_specular_color_;
	ogl::Uniform unif_specular_coef_;
	ogl::Uniform unif_light_position_;
	ogl::Uniform unif_bf_culling_;
	ogl::Uniform unif_layer_;
	ogl::Uniform unif_depth_texture_sampler_;
	ogl::Uniform unif_rgba_texture_sampler_;

public:

	using Self = ShaderPhongTransp;
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(ShaderPhongTransp);

	enum
	{
		ATTRIB_POS = 0, ATTRIB_NORM
	};

	/**
	 * @brief set current front color
	 * @param rgb
	 */
	void set_front_color(const Vector4f& rgb);

	/**
	 * @brief set current front color
	 * @param rgb
	 */
	void set_back_color(const Vector4f& rgb);

	/**
	 * @brief set current ambiant color
	 * @param rgb
	 */
	void set_ambiant_color(const Vector4f& rgb);

	/**
	 * @brief set current specular color
	 * @param rgb
	 */
	void set_specular_color(const Vector4f& rgb);

	/**
	 * @brief set current specular coefficient
	 * @param rgb
	 */
	void set_specular_coef(float32 coef);

	/**
	 * @brief set light position relative to screen
	 * @param l light position
	 */
	void set_light_position(const Vector3f& l);

	void set_bf_culling(bool cull);

	void set_layer(int layer);

	void set_rgba_sampler(GLint rgba_samp);

	void set_depth_sampler(GLint depth_samp);

	using Param = ShaderParamPhongTransp;

	static std::unique_ptr<Param> generate_param();

	static ShaderPhongTransp* get_instance();

private:

	ShaderPhongTransp();
	static ShaderPhongTransp* instance_;

};


} // namespace rendering

} // namespace cgogn

#endif // CGOGN_RENDERING_SHADER_TRANSP_PHONG_H_
