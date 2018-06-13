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


#include <cgogn/rendering/transparency_drawer.h>

namespace cgogn
{

namespace rendering
{

SurfaceTransparencyDrawer::~SurfaceTransparencyDrawer()
{
	param_flat_.reset();
	param_trq_.reset();
	fbo_layer_.reset();
	glDeleteQueries(1, &oq_transp_);
}

SurfaceTransparencyDrawer::SurfaceTransparencyDrawer():
	max_nb_layers_(8),
	param_flat_(nullptr),
	param_trq_(nullptr),
	fbo_layer_(nullptr),
	oq_transp_(0u),
	depthTexture_(0)
{
	param_flat_ = cgogn::rendering::ShaderFlatTransp::generate_param();
	param_flat_->front_color_ = Color(0,250,0,120);
	param_flat_->back_color_ = Color(0,0,250,120);
	param_flat_->ambiant_color_ = Color(0,0,0,0);

	param_phong_ = cgogn::rendering::ShaderPhongTransp::generate_param();
	param_phong_->front_color_ = Color(0,250,0,120);
	param_phong_->back_color_ = Color(0,0,250,120);
	param_phong_->ambiant_color_ = Color(0,0,0,0);
	param_phong_->specular_color_ = Color(255,255,255,0);
	param_phong_->specular_coef_ = 100.0f;

	param_trq_ = cgogn::rendering::ShaderTranspQuad::generate_param();

	param_copy_depth_ = ShaderCopyDepth::generate_param();

}

void SurfaceTransparencyDrawer::resize(int w, int h)
{
	width_ = w;
	height_ = h;

	texture_depth_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_depth_->bind();
	texture_depth_->setImage2D_simple(w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	texture_depth_->release();

	texture_color0_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color0_->bind();
	texture_color0_->setImage2D_simple(w, h, GL_RGBA, GL_RGBA, GL_FLOAT);
	texture_color0_->release();

	texture_color1_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color1_->bind();
	texture_color1_->setImage2D_simple(w, h, GL_R32F, GL_R32F, GL_FLOAT);
	texture_color1_->release();

	texture_color2_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color2_->bind();
	texture_color2_->setImage2D_simple(w, h, GL_R32F, GL_R32F, GL_FLOAT);
	texture_color2_->release();

	texture_color3_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color3_->bind();
	texture_color3_->setImage2D_simple(w, h, GL_RGBA, GL_RGBA, GL_FLOAT);
	texture_color3_->release();

	texture_color4_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color4_->bind();
	texture_color4_->setImage2D_simple(w, h, GL_R32F, GL_R32F, GL_FLOAT);
	texture_color4_->release();

	texture_color5_ = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color5_->bind();
	texture_color5_->setImage2D_simple(w, h, GL_RGBA, GL_RGBA, GL_FLOAT);
	texture_color5_->release();

	fbo_layer_ = cgogn::make_unique<ogl::Framebuffer>();
	fbo_layer_->attach(texture_depth_, GL_DEPTH_ATTACHMENT);
	fbo_layer_->attach(texture_color0_, GL_COLOR_ATTACHMENT0);
	fbo_layer_->attach(texture_color1_, GL_COLOR_ATTACHMENT1);
	fbo_layer_->attach(texture_color2_, GL_COLOR_ATTACHMENT2);
	fbo_layer_->attach(texture_color3_, GL_COLOR_ATTACHMENT3);
	fbo_layer_->attach(texture_color4_, GL_COLOR_ATTACHMENT4);
	fbo_layer_->attach(texture_color5_, GL_COLOR_ATTACHMENT5);

	if (!glIsQuery(oq_transp_))
		glGenQueries(1, &oq_transp_);

	if (glIsTexture(depthTexture_))
		glDeleteTextures(1, &depthTexture_);


	glGenTextures(1, &depthTexture_);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

}




} // namespace rendering

} // namespace cgogn
