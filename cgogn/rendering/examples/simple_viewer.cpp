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

#include "cgogn/rendering/opengl/all.h"

#include <QApplication>
#include <QMatrix4x4>
#include <QKeyEvent>

#include <QOGLViewer/qoglviewer.h>

#include <cgogn/core/cmap/cmap2.h>
//#include <cgogn/core/cmap/cmap2_tri.h>
//#include <cgogn/core/cmap/cmap2_quad.h>

#include <cgogn/io/map_import.h>

#include <cgogn/geometry/algos/bounding_box.h>
#include <cgogn/geometry/algos/normal.h>


#include <cgogn/rendering/map_render.h>
#include <cgogn/rendering/shaders/shader_simple_color.h>
#include <cgogn/rendering/shaders/shader_flat.h>
#include <cgogn/rendering/shaders/shader_phong.h>
#include <cgogn/rendering/shaders/shader_vector_per_vertex.h>

#include <cgogn/rendering/shaders/shader_bold_line.h>
#include <cgogn/rendering/shaders/shader_point_sprite.h>
#include <cgogn/rendering/shaders/shader_round_point.h>

#include <cgogn/geometry/algos/ear_triangulation.h>

#include <cgogn/rendering/drawer.h>

#include <cgogn/rendering/shaders/shader_blur.h>
#include <cgogn/rendering/shaders/shader_shadow.h>
#include <cgogn/rendering/shaders/shader_depth.h>
#include <cgogn/rendering/shaders/shader_shadow_blend.h>


#define DEFAULT_MESH_PATH CGOGN_STR(CGOGN_TEST_MESHES_PATH)

using Map2 = cgogn::CMap2;
//using Map2 = cgogn::CMap2Tri;
//using Map2 = cgogn::CMap2Quad;


using Vec3 = Eigen::Vector3d;
//using Vec3 = cgogn::geometry::Vec_T<std::array<double,3>>;

template <typename T>
using VertexAttribute = Map2::VertexAttribute<T>;

const int shadowMapResolution = 4096; 

class Viewer : public QOGLViewer
{
public:

	Viewer();
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(Viewer);

	void renderScene(QMatrix4x4 proj, QMatrix4x4 view, bool renderDepth, bool renderShadow, bool renderColor, QMatrix4x4 shadowMVP);
	virtual void draw();
	virtual void init();
	virtual void resizeGL(int w, int h);

	virtual void keyPressEvent(QKeyEvent *);
	void import(const std::string& surface_mesh);
	virtual ~Viewer();
	virtual void closeEvent(QCloseEvent *e);

private:

	Map2 map_;
	VertexAttribute<Vec3> vertex_position_;
	VertexAttribute<Vec3> vertex_normal_;

	cgogn::geometry::AABB<Vec3> bb_;

	std::unique_ptr<cgogn::rendering::MapRender> render_;

	std::unique_ptr<cgogn::rendering::VBO> vbo_pos_;
	std::unique_ptr<cgogn::rendering::VBO> vbo_norm_;
	std::unique_ptr<cgogn::rendering::VBO> vbo_color_;
	std::unique_ptr<cgogn::rendering::VBO> vbo_sphere_sz_;

	std::unique_ptr<cgogn::rendering::VBO> vbo_surface_pos_;
	std::unique_ptr<cgogn::rendering::ogl::Buffer> buffer_surface_indices_; 

	std::unique_ptr<cgogn::rendering::ShaderBoldLine::Param> param_edge_;
	std::unique_ptr<cgogn::rendering::ShaderFlat::Param> param_flat_;
	std::unique_ptr<cgogn::rendering::ShaderVectorPerVertex::Param> param_normal_;
	std::unique_ptr<cgogn::rendering::ShaderPhongColor::Param> param_phong_;
	std::unique_ptr<cgogn::rendering::ShaderPointSpriteColorSize::Param> param_point_sprite_;

	std::unique_ptr<cgogn::rendering::DisplayListDrawer> drawer_;
	std::unique_ptr<cgogn::rendering::DisplayListDrawer::Renderer> drawer_rend_;

	bool phong_rendering_;
	bool flat_rendering_;
	bool vertices_rendering_;
	bool edge_rendering_;
	bool normal_rendering_;
	bool bb_rendering_;
	bool pp_blur_; 

	int width_;
	int height_;

	std::unique_ptr<cgogn::rendering::shaders::Blur::Param> param_blur;
	std::unique_ptr<cgogn::rendering::shaders::Depth::Param> param_depth;
	std::unique_ptr<cgogn::rendering::shaders::Shadow::Param> param_shadowed;
	std::unique_ptr<cgogn::rendering::ShaderSimpleColor::Param> param_simplecolor;
	std::unique_ptr<cgogn::rendering::shaders::ShadowBlend::Param> param_shadow_blend;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_shadow_light_depth;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_shadow_light_depth;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_shadow_depth;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_shadow;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_shadow;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_color_preblend;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_depth_preblend;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_preblend;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_color_preblur;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_blur2;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_blur1;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_blur1;
};


//
// IMPLEMENTATION
//


void Viewer::import(const std::string& surface_mesh)
{
	cgogn::io::import_surface<Vec3>(map_, surface_mesh);

	vertex_position_ = map_.template get_attribute<Vec3, Map2::Vertex>("position");
	if (!vertex_position_.is_valid())
	{
		cgogn_log_error("Viewer::import") << "Missing attribute position. Aborting.";
		std::exit(EXIT_FAILURE);
	}

	vertex_normal_ = map_.template get_attribute<Vec3, Map2::Vertex>("normal");
	if (!vertex_normal_.is_valid())
	{
		vertex_normal_ = map_.template add_attribute<Vec3, Map2::Vertex>("normal");
		cgogn::geometry::compute_normal(map_, vertex_position_, vertex_normal_);
	}


// testing merge method
//	Map2 map2;
//	cgogn::io::import_surface<Vec3>(map2, std::string(DEFAULT_MESH_PATH) + std::string("off/star_convex.off"));
//	map_.merge(map2);

	cgogn::geometry::compute_AABB(vertex_position_, bb_);
	setSceneRadius(bb_.diag_size()*10.0);
	Vec3 center = bb_.center();
	setSceneCenter(qoglviewer::Vec(center[0], center[1], center[2]));
	showEntireScene();
}

Viewer::~Viewer()
{}

void Viewer::closeEvent(QCloseEvent*)
{
	render_.reset();
	vbo_pos_.reset();
	vbo_norm_.reset();
	vbo_color_.reset();
	vbo_sphere_sz_.reset();
	drawer_.reset();
	drawer_rend_.reset();
	cgogn::rendering::ogl::ShaderProgram::clean_all();

}

Viewer::Viewer() :
	map_(),
	vertex_position_(),
	vertex_normal_(),
	bb_(),
	render_(nullptr),
	vbo_pos_(nullptr),
	vbo_norm_(nullptr),
	vbo_color_(nullptr),
	vbo_sphere_sz_(nullptr),
	drawer_(nullptr),
	drawer_rend_(nullptr),
	phong_rendering_(true),
	flat_rendering_(false),
	vertices_rendering_(false),
	edge_rendering_(false),
	normal_rendering_(false),
	bb_rendering_(true),
	pp_blur_(false)
{}

void Viewer::keyPressEvent(QKeyEvent *ev)
{
	switch (ev->key())
	{
		case Qt::Key_P:
			phong_rendering_ = true;
			flat_rendering_ = false;
			break;
		case Qt::Key_F:
			flat_rendering_ = true;
			phong_rendering_ = false;
			break;
		case Qt::Key_N:
			normal_rendering_ = !normal_rendering_;
			break;
		case Qt::Key_E:
			edge_rendering_ = !edge_rendering_;
			break;
		case Qt::Key_V:
			vertices_rendering_ = !vertices_rendering_;
			break;
		case Qt::Key_B:
			bb_rendering_ = !bb_rendering_;
			break;
		case Qt::Key_X:
			pp_blur_ = !pp_blur_;
			break;
		default:
			break;
	}
	// enable QGLViewer keys
	QOGLViewer::keyPressEvent(ev);
	//update drawing
	update();
}

void Viewer::renderScene(QMatrix4x4 proj, QMatrix4x4 view, bool renderDepth, bool renderShadow, bool renderColor, QMatrix4x4 shadowMVP)
{
	if (renderDepth)
	{
		param_depth->set_position_vbo(vbo_surface_pos_.get());
		param_depth->bind(proj.data(), view.data());
		buffer_surface_indices_->bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
		buffer_surface_indices_->release();
		param_depth->release();
	}
	else if (renderShadow)
	{
		texture_shadow_light_depth->bindAt(1);
		param_shadowed->set_position_vbo(vbo_surface_pos_.get());
		param_shadowed->bind(proj.data(), view.data());
		param_shadowed->set_shadowMap(texture_shadow_light_depth->slot());
		param_shadowed->set_shadowMVP(shadowMVP.data());
		buffer_surface_indices_->bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
		buffer_surface_indices_->release();
		param_shadowed->release();
	}
	else if (renderColor)
	{
		param_simplecolor->set_position_vbo(vbo_surface_pos_.get());
		param_simplecolor->bind(proj.data(), view.data());
		buffer_surface_indices_->bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
		buffer_surface_indices_->release();
		param_simplecolor->release();
	}

	float offset = 35.0f;
	QMatrix4x4 m = QMatrix4x4();
	m.setToIdentity();
	m.translate(-5.0f*offset, -5.0f*offset, 0.0f);
	for (int x = 0; x < 11; x++)
	{
		for (int y = 0; y < 11; y++)
		{
			auto mv = view * m; 
			auto smv = shadowMVP * m;
			if (renderDepth)
			{
				param_depth->set_position_vbo(vbo_pos_.get());
				param_depth->bind(proj.data(), mv.data());
				render_->draw(cgogn::rendering::TRIANGLES);
				param_depth->release();
			}
			else if (renderShadow)
			{
				texture_shadow_light_depth->bindAt(1);
				param_shadowed->set_position_vbo(vbo_pos_.get());
				param_shadowed->bind(proj.data(), mv.data());
				param_shadowed->set_shadowMap(texture_shadow_light_depth->slot());
				param_shadowed->set_shadowMVP(smv.data());
				render_->draw(cgogn::rendering::TRIANGLES);
				param_shadowed->release();
			}
			else if (renderColor)
			{
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0f, 2.0f);
				if (flat_rendering_)
				{
					param_flat_->bind(proj.data(), mv.data());
					render_->draw(cgogn::rendering::TRIANGLES);
					param_flat_->release();
				}

				if (phong_rendering_)
				{
					param_phong_->bind(proj.data(), mv.data());
					render_->draw(cgogn::rendering::TRIANGLES);
					param_phong_->release();
				}
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
			m.translate(0.0f, offset, 0.0f);
		}
		m.translate(offset, -11.0f*offset, 0.0f);
	}

	m.setToIdentity();
	m.translate(0.0f, 0.0f, 90.0f);
	m.scale(5.0f); 
	auto mv = view * m;
	auto smv = shadowMVP * m; 
	if (renderDepth)
	{
		param_depth->set_position_vbo(vbo_pos_.get());
		param_depth->bind(proj.data(), mv.data());
		render_->draw(cgogn::rendering::TRIANGLES);
		param_depth->release();
	}
	else if (renderShadow)
	{
		texture_shadow_light_depth->bindAt(1);
		param_shadowed->set_position_vbo(vbo_pos_.get());
		param_shadowed->bind(proj.data(), mv.data());
		param_shadowed->set_shadowMap(texture_shadow_light_depth->slot());
		param_shadowed->set_shadowMVP(smv.data());
		render_->draw(cgogn::rendering::TRIANGLES);
		param_shadowed->release();
	}
	else if (renderColor)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 2.0f);
		if (flat_rendering_)
		{
			param_flat_->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_flat_->release();
		}

		if (phong_rendering_)
		{
			param_phong_->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_phong_->release();
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
	}


	/*
	if (vertices_rendering_)
	{
		param_point_sprite_->bind(proj.data(), view.data());;
		render_->draw(cgogn::rendering::POINTS);
		param_point_sprite_->release();
	}

	if (edge_rendering_)
	{
		param_edge_->bind(proj.data(), view.data());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		render_->draw(cgogn::rendering::LINES);
		glDisable(GL_BLEND);
		param_edge_->release();
	}

	if (normal_rendering_)
	{
		param_normal_->bind(proj.data(), view.data());;
		render_->draw(cgogn::rendering::POINTS);
		param_normal_->release();
	}

	if (bb_rendering_)
		drawer_rend_->draw(cgogn::Matrix4f(proj.data()), cgogn::Matrix4f(view.data()));*/
}

void Viewer::draw()
{
	QMatrix4x4 proj;
	camera()->getProjectionMatrix(proj);

	QMatrix4x4 view;
	camera()->getModelViewMatrix(view);

	float shadowFrustrum = 200; 
	QMatrix4x4 lightProj;
	lightProj.ortho(-shadowFrustrum, shadowFrustrum, -shadowFrustrum, shadowFrustrum, -shadowFrustrum, shadowFrustrum);

	QMatrix4x4 lightView;
	lightView.lookAt(QVector3D(0.0f, 0.0f, 10.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); 

	QMatrix4x4 lightVPbiased = QMatrix4x4(0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f) * lightProj * lightView;

	fbo_shadow_light_depth->bind();
	glViewport(0, 0, shadowMapResolution, shadowMapResolution);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(lightProj, lightView, true, false, false, QMatrix4x4());
	fbo_shadow_light_depth->release();

	fbo_shadow->bind();
	glViewport(0, 0, width_, height_);
	glClearColor(1.0f,0.0f,0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(proj, view, false, true, false, lightVPbiased);
	fbo_shadow->release();

	fbo_preblend->bind();

	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(proj, view, false, false, true, QMatrix4x4());
		
	texture_shadow->bindAt(1);

	fbo_preblend->release();

	if (pp_blur_)
	{
		//Pass blur 1
		fbo_blur1->bind();

		texture_shadow->bindAt(0);
		texture_depth_preblend->bindAt(1);

		param_blur->bind();
		param_blur->set_rgba_sampler(texture_shadow->slot());
		param_blur->set_depth_filter(texture_depth_preblend->slot());
		param_blur->set_blur_dimension(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		param_blur->release();

		fbo_blur1->release();

		//Pass blur 2
		fbo_blur2->bind();

		texture_blur1->bindAt(0);
		texture_depth_preblend->bindAt(1); 

		param_blur->bind();
		param_blur->set_rgba_sampler(texture_blur1->slot());
		param_blur->set_depth_filter(texture_depth_preblend->slot());
		param_blur->set_blur_dimension(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		param_blur->release();

		fbo_blur2->release();

	}

	texture_color_preblend->bindAt(0);
	texture_shadow->bindAt(1);

	param_shadow_blend->bind();
	param_shadow_blend->set_rgba_sampler1(texture_color_preblend->slot());
	param_shadow_blend->set_rgba_sampler2(texture_shadow->slot());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	param_shadow_blend->release();

}

void Viewer::init()
{
	glewInit();

	glClearColor(0.1f,0.1f,0.3f,0.0f);

	// create and fill VBO for positions
	vbo_pos_ = cgogn::make_unique<cgogn::rendering::VBO>(3);
	cgogn::rendering::update_vbo(vertex_position_, vbo_pos_.get());

	// create and fill VBO for normals
	vbo_norm_ = cgogn::make_unique<cgogn::rendering::VBO>(3);
	cgogn::rendering::update_vbo(vertex_normal_, vbo_norm_.get());

	// fill a color vbo with abs of normals
	vbo_color_ = cgogn::make_unique<cgogn::rendering::VBO>(3);
	cgogn::rendering::update_vbo(vertex_normal_, vbo_color_.get(), [] (const Vec3& n) -> std::array<float,3>
	{
		return {float(std::abs(n[0])), float(std::abs(n[1])), float(std::abs(n[2]))};
	});

	// fill a sphere size vbo
	vbo_sphere_sz_ = cgogn::make_unique<cgogn::rendering::VBO>(1);
	cgogn::rendering::update_vbo(vertex_normal_, vbo_sphere_sz_.get(), [&] (const Vec3& n) -> float
	{
		return bb_.diag_size()/1000.0 * (1.0 + 2.0*std::abs(n[2]));
	});


	vbo_surface_pos_ = cgogn::make_unique<cgogn::rendering::VBO>(3);
	vbo_surface_pos_->allocate(8, 3); 
	float plane_size = 300.0f;
	cgogn::Vector3f baseOffset = cgogn::Vector3f(0.0f, 0.0f, -65.0f);
	cgogn::Vector3f points[8] = 
	{
		baseOffset + cgogn::Vector3f(-1.0f,-1.0f,0.0f)*plane_size, 
		baseOffset + cgogn::Vector3f(1.0f,-1.0f,0.0f)*plane_size,
		baseOffset + cgogn::Vector3f(1.0f,1.0f,0.0f)*plane_size,
		baseOffset + cgogn::Vector3f(-1.0f,1.0f,0.0f)*plane_size,
		baseOffset + cgogn::Vector3f(-1.0f,-1.0f,-0.02f)*plane_size,
		baseOffset + cgogn::Vector3f(1.0f,-1.0f,-0.02f)*plane_size,
		baseOffset + cgogn::Vector3f(1.0f,1.0f,-0.02f)*plane_size,
		baseOffset + cgogn::Vector3f(-1.0f,1.0f,-0.02f)*plane_size,
	};
	vbo_surface_pos_->bind(); 
	vbo_surface_pos_->copy_data(0, 8 * sizeof(cgogn::Vector3f), points); 
	vbo_surface_pos_->release();

	buffer_surface_indices_ = cgogn::make_unique<cgogn::rendering::ogl::Buffer>(GL_ELEMENT_ARRAY_BUFFER);
	buffer_surface_indices_->create();
	buffer_surface_indices_->setUsagePattern(GL_STATIC_DRAW);
	buffer_surface_indices_->bind(); 
	uint32_t surface_indices[] = 
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		0, 1, 5, 5, 4, 0,
		2, 6, 5, 5, 1, 2, 
		2, 3, 7, 7, 6, 2,
		0, 4, 7, 7, 3, 0
	};
	buffer_surface_indices_->allocate(surface_indices, 36 * sizeof(uint32_t));
	buffer_surface_indices_->release(); 

	cgogn::rendering::update_vbo(vertex_position_, vbo_pos_.get());

	// map rendering object (primitive creation & sending to GPU)
	render_ = cgogn::make_unique<cgogn::rendering::MapRender>();
	render_->init_primitives(map_, cgogn::rendering::POINTS);
	render_->init_primitives(map_, cgogn::rendering::LINES);
	render_->init_primitives(map_, cgogn::rendering::TRIANGLES, &vertex_position_);

	// generation of one parameter set (for this shader) : vbo + uniforms
	param_point_sprite_ = cgogn::rendering::ShaderPointSpriteColorSize::generate_param();
	// set vbo param (see param::set_vbo signature)
	param_point_sprite_->set_all_vbos(vbo_pos_.get(), vbo_color_.get(), vbo_sphere_sz_.get());
	// set uniforms data

	param_blur = cgogn::rendering::shaders::Blur::generate_param();
	param_shadowed = cgogn::rendering::shaders::Shadow::generate_param();
	param_depth = cgogn::rendering::shaders::Depth::generate_param();
	param_simplecolor = cgogn::rendering::ShaderSimpleColor::generate_param();
	param_simplecolor->color_ = cgogn::Color(128, 128, 128); 
	param_shadow_blend = cgogn::rendering::shaders::ShadowBlend::generate_param();

	param_edge_ = cgogn::rendering::ShaderBoldLine::generate_param();
	param_edge_->set_position_vbo(vbo_pos_.get());
	param_edge_->color_ = cgogn::Color(255,255,0);
	param_edge_->width_= 2.5f;

	param_flat_ = cgogn::rendering::ShaderFlat::generate_param();
	param_flat_->set_position_vbo(vbo_pos_.get());
	param_flat_->front_color_ = cgogn::Color(0,200,0);
	param_flat_->back_color_ = cgogn::Color(0,0,200);
	param_flat_->ambiant_color_ = cgogn::Color(5,5,5);

	param_normal_ = cgogn::rendering::ShaderVectorPerVertex::generate_param();
	param_normal_->set_all_vbos(vbo_pos_.get(), vbo_norm_.get());
	param_normal_->color_ = cgogn::Color(200,0,200);
	param_normal_->length_ = bb_.diag_size()/50;

	param_phong_ = cgogn::rendering::ShaderPhongColor::generate_param();
	param_phong_->set_all_vbos(vbo_pos_.get(), vbo_norm_.get(), vbo_color_.get());


	// drawer for simple old-school g1 rendering
	drawer_ = cgogn::make_unique<cgogn::rendering::DisplayListDrawer>();
	drawer_rend_= drawer_->generate_renderer();
	drawer_->new_list();
	drawer_->line_width_aa(2.0);
	drawer_->begin(GL_LINE_LOOP);
		drawer_->color3f(1.0,1.0,1.0);
		drawer_->vertex3f(bb_.min()[0],bb_.min()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.min()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.max()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.max()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.max()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.max()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.min()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.min()[1],bb_.max()[2]);
	drawer_->end();
	drawer_->begin(GL_LINES);
	drawer_->color3f(1.0,1.0,1.0);
		drawer_->vertex3f(bb_.min()[0],bb_.min()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.max()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.min()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.min()[0],bb_.max()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.min()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.min()[1],bb_.max()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.max()[1],bb_.min()[2]);
		drawer_->vertex3f(bb_.max()[0],bb_.max()[1],bb_.max()[2]);
	drawer_->end();
	drawer_->end_list();
}

void Viewer::resizeGL(int w, int h)
{
	width_ = w;
	height_ = h;
	
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &cgogn::rendering::ogl::Framebuffer::qtDefaultFramebuffer);

	texture_shadow_light_depth = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_shadow_light_depth->bind();
	texture_shadow_light_depth->setImage2D_simple(shadowMapResolution, shadowMapResolution, GL_DEPTH_COMPONENT, GL_FLOAT);
	texture_shadow_light_depth->release();

	fbo_shadow_light_depth = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_shadow_light_depth->bind();
	fbo_shadow_light_depth->attach(texture_shadow_light_depth, GL_DEPTH_ATTACHMENT);
	fbo_shadow_light_depth->check();
	fbo_shadow_light_depth->release();

	texture_shadow_depth = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_shadow_depth->bind();
	texture_shadow_depth->setImage2D_simple(w, h, GL_DEPTH_COMPONENT, GL_FLOAT);
	texture_shadow_depth->release();

	texture_shadow = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_shadow->bind();
	texture_shadow->setImage2D_simple(w, h, GL_RED, GL_FLOAT);
	texture_shadow->release();

	fbo_shadow = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_shadow->bind();
	fbo_shadow->attach(texture_shadow_depth, GL_DEPTH_ATTACHMENT);
	fbo_shadow->attach(texture_shadow, GL_COLOR_ATTACHMENT0);
	fbo_shadow->check();
	fbo_shadow->release();

	texture_color_preblend = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color_preblend->bind();
	texture_color_preblend->setImage2D_simple(w, h, GL_RGBA, GL_FLOAT);
	texture_color_preblend->release();

	texture_depth_preblend = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_depth_preblend->bind();
	texture_depth_preblend->setImage2D_simple(w, h, GL_DEPTH_COMPONENT, GL_FLOAT);
	texture_depth_preblend->release();

	fbo_preblend = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_preblend->bind();
	fbo_preblend->attach(texture_depth_preblend, GL_DEPTH_ATTACHMENT);
	fbo_preblend->attach(texture_color_preblend, GL_COLOR_ATTACHMENT0);
	fbo_preblend->check();
	fbo_preblend->release();

	texture_color_preblur = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color_preblur->bind();
	texture_color_preblur->setImage2D_simple(w, h, GL_RGBA, GL_FLOAT);
	texture_color_preblur->release();

	fbo_blur2 = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_blur2->bind();
	fbo_blur2->attach(texture_shadow, GL_COLOR_ATTACHMENT0);
	fbo_blur2->check();
	fbo_blur2->release();

	texture_blur1 = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_blur1->bind();
	texture_blur1->setImage2D_simple(w, h, GL_RGBA, GL_FLOAT);
	texture_blur1->release();

	fbo_blur1 = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_blur1->bind();
	fbo_blur1->attach(texture_blur1, GL_COLOR_ATTACHMENT0);
	fbo_blur1->check();
	fbo_blur1->release();

	QOGLViewer::resizeGL(w, h);
}

int main(int argc, char** argv)
{
	std::string surface_mesh;
	if (argc < 2)
	{
		cgogn_log_info("simple_viewer") << "USAGE: " << argv[0] << " [filename]";
		surface_mesh = std::string(DEFAULT_MESH_PATH) + std::string("off/aneurysm_3D.off");
		cgogn_log_info("simple_viewer") << "Using default mesh \"" << surface_mesh << "\".";
	}
	else
		surface_mesh = std::string(argv[1]);

	QApplication application(argc, argv);
	qoglviewer::init_ogl_context(); 

	// Instantiate the viewer.
	Viewer viewer;
	viewer.setWindowTitle("simple_viewer");
	viewer.import(surface_mesh);
	viewer.show();

	// Run main loop.
	return application.exec();
}
