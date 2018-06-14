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

#define CGOGN_RENDER_SHADERS_EXPLODE_VOLUME_CPP_

#include <iostream>

#include <cgogn/rendering/shaders/shader_explode_volumes.h>



namespace cgogn
{

namespace rendering
{

const char* ShaderExplodeVolumesGen::vertex_shader_source_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderExplodeVolumesGen::geometry_shader_source_ =
"#version 150\n"
"layout (lines_adjacency) in;\n"
"layout (triangle_strip, max_vertices=3) out;\n"
"out vec3 color_f;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform mat3 normal_matrix;\n"
"uniform float explode_vol;\n"
"uniform vec3 light_position;\n"
"uniform vec4 color;\n"
"uniform vec4 plane_clip;\n"
"uniform vec4 plane_clip2;\n"
"void main()\n"
"{\n"
"	float d = dot(plane_clip,gl_in[0].gl_Position);\n"
"	float d2 = dot(plane_clip2,gl_in[0].gl_Position);\n"
"	if ((d<=0.0)&&(d2<=0.0))\n"
"	{\n"
"		vec3 v1 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;\n"
"		vec3 v2 = gl_in[3].gl_Position.xyz - gl_in[1].gl_Position.xyz;\n"
"		vec3 N  = normalize(normal_matrix*cross(v1,v2));\n"
"		vec4 face_center =  model_view_matrix * gl_in[1].gl_Position;\n"
"		vec3 L =  normalize (light_position - face_center.xyz);\n"
"		float lambertTerm = abs(dot(N,L));\n"
"		for (int i=1; i<=3; i++)\n"
"		{\n"
"			vec4 Q = explode_vol *  gl_in[i].gl_Position  + (1.0-explode_vol) * gl_in[0].gl_Position;\n"
"			gl_Position = projection_matrix * model_view_matrix *  Q;\n"
"			color_f = color.rgb * lambertTerm;\n"
"			EmitVertex();\n"
"		}\n"
"		EndPrimitive();\n"
"	}\n"
"}\n";

const char* ShaderExplodeVolumesGen::fragment_shader_source_ =
"#version 150\n"
"in vec3 color_f;\n"
"out vec3 fragColor;\n"
"void main()\n"
"{\n"
"   fragColor = color_f;\n"
"}\n";

const char* ShaderExplodeVolumesGen::vertex_shader_source2_ =
"#version 150\n"
"in vec3 vertex_pos;\n"
"in vec3 vertex_color;\n"
"out vec3 color_v;\n"
"void main()\n"
"{\n"
"   color_v = vertex_color;\n"
"   gl_Position = vec4(vertex_pos,1.0);\n"
"}\n";

const char* ShaderExplodeVolumesGen::geometry_shader_source2_ =
"#version 150\n"
"layout (lines_adjacency) in;\n"
"layout (triangle_strip, max_vertices=3) out;\n"
"in vec3 color_v[];\n"
"out vec3 color_f;\n"
"uniform mat4 projection_matrix;\n"
"uniform mat4 model_view_matrix;\n"
"uniform mat3 normal_matrix;\n"
"uniform float explode_vol;\n"
"uniform vec3 light_position;\n"
"uniform vec4 plane_clip;\n"
"uniform vec4 plane_clip2;\n"
"void main()\n"
"{\n"
"	float d = dot(plane_clip,gl_in[0].gl_Position);\n"
"	float d2 = dot(plane_clip,gl_in[0].gl_Position);\n"
"	if ((d<=0.0)&&(d2<=0.0))\n"
"	{\n"
"		vec3 v1 = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;\n"
"		vec3 v2 = gl_in[3].gl_Position.xyz - gl_in[1].gl_Position.xyz;\n"
"		vec3 N  = normalize(normal_matrix*cross(v1,v2));\n"
"		vec4 face_center =  model_view_matrix * gl_in[1].gl_Position;\n"
"		vec3 L =  normalize (light_position - face_center.xyz);\n"
"		float lambertTerm = abs(dot(N,L));\n"
"		for (int i=1; i<=3; i++)\n"
"		{\n"
"			vec4 Q = explode_vol *  gl_in[i].gl_Position  + (1.0-explode_vol) * gl_in[0].gl_Position;\n"
"			gl_Position = projection_matrix * model_view_matrix *  Q;\n"
"			color_f = color_v[i]*lambertTerm;\n"
"			EmitVertex();\n"
"		}\n"
"		EndPrimitive();\n"
"	}\n"
"}\n";

const char* ShaderExplodeVolumesGen::fragment_shader_source2_ =
"#version 150\n"
"in vec3 color_f;\n"
"out vec3 fragColor;\n"
"void main()\n"
"{\n"
"   fragColor = color_f;\n"
"}\n";

ShaderExplodeVolumesGen::ShaderExplodeVolumesGen(bool color_per_vertex)
{
	if (color_per_vertex)
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source2_);
		addShader(GL_GEOMETRY_SHADER, geometry_shader_source2_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source2_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
		bindAttributeLocation("vertex_color", ATTRIB_COLOR);
	}
	else
	{
		addShader(GL_VERTEX_SHADER, vertex_shader_source_);
		addShader(GL_GEOMETRY_SHADER, geometry_shader_source_);
		addShader(GL_FRAGMENT_SHADER, fragment_shader_source_);
		bindAttributeLocation("vertex_pos", ATTRIB_POS);
	}

	link();

	get_matrices_uniforms();
	unif_expl_v_ = "explode_vol";
	unif_plane_clip_ = "plane_clip";
	unif_plane_clip2_ = "plane_clip2";
	unif_light_position_ = "light_position";
	unif_color_ = "color";

	// default param
	bind();
	set_light_position(Vector3f(10.0f,100.0f,1000.0f));
	set_explode_volume(0.8f);
	set_color(Color(255,0,0));
	set_plane_clip(Vector4f(0,0,0,0));
	set_plane_clip2(Vector4f(0,0,0,0));
	release();
}

void ShaderExplodeVolumesGen::set_color(const Color& rgb)
{
	if (unif_color_.found())
		unif_color_.set(rgb);
}

void ShaderExplodeVolumesGen::set_light_position(const Vector3f& l)
{
	unif_light_position_.set(l);
}

void ShaderExplodeVolumesGen::set_explode_volume(float32 x)
{
	unif_expl_v_.set(x);
}

void ShaderExplodeVolumesGen::set_plane_clip(const Vector4f& plane)
{
	unif_plane_clip_.set(plane);
}


void ShaderExplodeVolumesGen::set_plane_clip2(const Vector4f& plane)
{
	unif_plane_clip2_.set(plane);
}


template class CGOGN_RENDERING_API ShaderExplodeVolumesTpl<false>;
template class CGOGN_RENDERING_API ShaderExplodeVolumesTpl<true>;
template class CGOGN_RENDERING_API ShaderParamExplodeVolumes<false>;
template class CGOGN_RENDERING_API ShaderParamExplodeVolumes<true>;

} // namespace rendering

} // namespace cgogn
