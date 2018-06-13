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

#include <iostream>

#include <cgogn/rendering/transparency_shaders/shader_transparent_phong.h>


namespace cgogn
{

namespace rendering
{


const char* ShaderPhongTransp::vertex_shader_source_ =
"#version 330\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_normal;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform mat3 normal_matrix;\n"
"out vec3 Normal;\n"
"out vec3 pos;\n"
"out vec4 projCoord;\n"
"void main()\n"
"{\n"
"	Normal = normal_matrix * vertex_normal;\n"
"	vec4 pos4 = model_view_matrix * vec4(vertex_pos,1.0);\n"
"	pos = pos4.xyz;"
"	gl_Position = projection_matrix * pos4;\n"
"   projCoord = gl_Position;\n"
"}\n";

const char* ShaderPhongTransp::fragment_shader_source_ =
"#version 330\n"
"layout(location = 0) out vec4 color_out;\n"
"layout(location = 1) out float depth_out;\n"
"in vec3 pos;\n"
"in vec4 projCoord;\n"
"in vec3 Normal;\n"
"uniform vec4 front_color;\n"
"uniform vec4 back_color;\n"
"uniform vec4 ambiant_color;\n"
"uniform vec4 spec_color;\n"
"uniform float spec_coef;\n"
"uniform vec3 lightPosition;\n"
"uniform sampler2D rgba_texture ;\n"
"uniform sampler2D depth_texture ;\n"
"uniform int layer;\n"
"uniform bool cull_back_face;\n"
"void main()\n"
"{\n"
"	vec3 EyeVector = -pos;"
"	vec3 N = normalize (Normal);\n"
"	vec3 L = normalize (lightPosition - pos);\n"
"	vec3 E = normalize(-pos);\n"
"	vec3 R = reflect(-L, N);\n"
"	float specular = pow( max(dot(R, E), 0.0), spec_coef );\n"

"	vec3 tc = 0.5*projCoord.xyz/projCoord.w + vec3(0.5,0.5,0.5);\n"
"	if ((layer>0) && (tc.z <= texture(depth_texture, tc.xy).r)) discard;\n"
"	if (gl_FrontFacing==false)\n" // do not use ! because of bug on old intel under OS/X
"	{\n"
"		N *= -1.0;\n"
"	}\n"
"	float lambert = clamp(dot(N,L),0.0,1.0);\n"
"	vec3 color;\n"
"	float alpha;\n"
"	if (gl_FrontFacing)\n"
"		{ color = ambiant_color.rgb+lambert*front_color.rgb+spec_color.rgb*specular; alpha = front_color.a;}\n"
"	else \n"
"		if (cull_back_face) discard;\n"
"		 else { color = ambiant_color.rgb+lambert*back_color.rgb+spec_color.rgb*specular; alpha = back_color.a;}\n"
"	vec4 dst = texture(rgba_texture, tc.xy);\n"
"	color_out = vec4( (dst.a * (alpha * color) + dst.rgb), (1.0-alpha)*dst.a);"
"	depth_out = tc.z;\n"
"}\n";

ShaderPhongTransp* ShaderPhongTransp::instance_ = nullptr;

ShaderPhongTransp::ShaderPhongTransp()
{
	addShader(GL_VERTEX_SHADER, vertex_shader_source_);
	addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
	bindAttributeLocation("vertex_pos", ATTRIB_POS);
	link();

	bind();
	get_matrices_uniforms();

	unif_front_color_ = "front_color";
	unif_back_color_ = "back_color";
	unif_ambiant_color_ = "ambiant_color";
	unif_specular_color_ = "spec_color";
	unif_specular_coef_ = "spec_coef";

	unif_light_position_ = "lightPosition";
	unif_layer_ = "layer";
	unif_bf_culling_ = "cull_back_face";
	unif_rgba_texture_sampler_ ="rgba_texture";
	unif_depth_texture_sampler_ = "depth_texture";

	release(); 
}

void ShaderPhongTransp::set_light_position(const Vector3f& l)
{
	unif_light_position_.set(l);
}

void ShaderPhongTransp::set_front_color(const Vector4f& rgb)
{
	if (unif_front_color_.found())
		unif_front_color_.set(rgb);
}

void ShaderPhongTransp::set_back_color(const Vector4f& rgb)
{
	if (unif_back_color_.found())
		unif_back_color_.set(rgb);
}

void ShaderPhongTransp::set_ambiant_color(const Vector4f& rgb)
{
	unif_ambiant_color_.set(rgb);
}

void ShaderPhongTransp::set_specular_color(const Vector4f& rgb)
{
	unif_specular_color_.set(rgb);
}

void ShaderPhongTransp::set_specular_coef(GLfloat coef)
{
	unif_specular_coef_.set(coef);
}


void ShaderPhongTransp::set_bf_culling(bool cull)
{
	unif_bf_culling_.set(cull);
}


void ShaderPhongTransp::set_layer(int layer)
{
	unif_layer_.set(layer);
}

void ShaderPhongTransp::set_rgba_sampler(GLint rgba_samp)
{
	unif_rgba_texture_sampler_.set(rgba_samp);
}

void ShaderPhongTransp::set_depth_sampler(GLint depth_samp)
{
	unif_depth_texture_sampler_.set(depth_samp);
}


std::unique_ptr< ShaderPhongTransp::Param> ShaderPhongTransp::generate_param()
{
	if (!instance_)
	{
		instance_ = new ShaderPhongTransp();
		ShaderProgram::register_instance(instance_);
	}
	return cgogn::make_unique<Param>(instance_);
}

ShaderPhongTransp* ShaderPhongTransp::get_instance()
{
	if (!instance_)
	{
		instance_ = new ShaderPhongTransp();
		ShaderProgram::register_instance(instance_);
	}
	return instance_;
}

ShaderParamPhongTransp::ShaderParamPhongTransp(ShaderPhongTransp* sh) :
	ogl::ShaderParam(sh),
	front_color_(Color(250, 0, 0)),
	back_color_(Color(0, 250, 0)),
	ambiant_color_(Color(5, 5, 5)),
	specular_color_(Color(255, 255, 255)),
	specular_coef_(100),
	light_pos_(Vector3f(10, 100, 1000)),
	bf_culling_(false)
{}

void ShaderParamPhongTransp::set_position_vbo(VBO* vbo_pos)
{
	
	program->bind();
	vao_->bind();
	vao_->attribPointer(ShaderPhongTransp::ATTRIB_POS, vbo_pos, GL_FLOAT);
	vao_->release();
	program->release();
}

void ShaderParamPhongTransp::set_normal_vbo(VBO* vbo_normal)
{
	program->bind();
	vao_->bind();
	vao_->attribPointer(ShaderPhongTransp::ATTRIB_POS, vbo_normal, GL_FLOAT);
	vao_->release();
	program->release();
}



void ShaderParamPhongTransp::set_uniforms()
{
	ShaderPhongTransp* sh = static_cast<ShaderPhongTransp*>(this->program);
	sh->set_front_color(front_color_);
	sh->set_back_color(back_color_);
	sh->set_ambiant_color(ambiant_color_);
	sh->set_specular_color(specular_color_);
	sh->set_specular_coef(specular_coef_);
	sh->set_light_position(light_pos_);
	sh->set_bf_culling(bf_culling_);
}

void ShaderParamPhongTransp::set_alpha(int alpha)
{
	front_color_.w() = alpha / 255.0f;
	back_color_.w() = alpha / 255.0f;
}

} // namespace rendering

} // namespace cgogn
