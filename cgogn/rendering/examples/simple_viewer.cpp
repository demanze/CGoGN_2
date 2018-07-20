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
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QSlider>

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
#include <cgogn/rendering/shaders/shader_border.h>
#include <cgogn/rendering/shaders/shader_scene_data.h>
#include <cgogn/rendering/shaders/shader_depth.h>
#include <cgogn/rendering/shaders/shader_light_blend.h>


#define DEFAULT_MESH_PATH CGOGN_STR(CGOGN_TEST_MESHES_PATH)

using Map2 = cgogn::CMap2;
//using Map2 = cgogn::CMap2Tri;
//using Map2 = cgogn::CMap2Quad;

using Vec3 = Eigen::Vector3d;
//using Vec3 = cgogn::geometry::Vec_T<std::array<double,3>>;

template <typename T>
using VertexAttribute = Map2::VertexAttribute<T>;

const int shadowMapResolution = 4096; 
bool useGargoyle = false;

class Viewer : public QOGLViewer
{
public:

	Viewer();
	CGOGN_NOT_COPYABLE_NOR_MOVABLE(Viewer);

	void renderModel(QMatrix4x4 proj, QMatrix4x4 modelView, std::unique_ptr<cgogn::rendering::VBO>& vbo_pos, std::unique_ptr<cgogn::rendering::VBO>& vbo_norm); 
	void renderScene(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 shadowMVP);
	
	virtual void draw();
	virtual void init();
	virtual void resizeGL(int w, int h);

	virtual void keyPressEvent(QKeyEvent *);
	void import(const std::string& surface_mesh);
	void addWidgets(); 

	virtual ~Viewer();
	virtual void closeEvent(QCloseEvent *e);

	QCheckBox* checkBoxFlatLighting;
	QCheckBox* checkBoxShadowMapping;
	QCheckBox* checkBoxSSAO;
	QCheckBox* checkBoxBorder;
	QCheckBox* checkBoxBlur;

	QSlider* sliderRadiusSSAO;
	QSlider* sliderRadiusBorder; 

	QRadioButton* radioButtonShowColor;
	QRadioButton* radioButtonShowNormal;

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
	std::unique_ptr<cgogn::rendering::VBO> vbo_surface_norm_;
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
	std::unique_ptr<cgogn::rendering::shaders::Shadow::Param> param_shadow;
	std::unique_ptr<cgogn::rendering::shaders::Border::Param> param_border;
	std::unique_ptr<cgogn::rendering::shaders::SceneData::Param> param_scene_data;
	std::unique_ptr<cgogn::rendering::ShaderSimpleColor::Param> param_simplecolor;
	std::unique_ptr<cgogn::rendering::shaders::LightBlend::Param> param_light_blend;

	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_shadow_map;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_position;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_normal;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_depth_temp;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_light;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_border;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_color;
	std::unique_ptr<cgogn::rendering::ogl::Texture> texture_blur1;

	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_shadow_map;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_shadow;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_border;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_scene_data;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_color;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_blur1;
	std::unique_ptr<cgogn::rendering::ogl::Framebuffer> fbo_blur2;

	enum RenderMode
	{
		ShadowMap,
		Gbuffer,
		Shadow,
		SSAO,
		Border,
		Color, 
	} renderMode; 

	std::unique_ptr<cgogn::rendering::ogl::Texture> ssao_noiseTexture;
	static const int noiseSize = 64; 
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
	setSceneRadius(1000.0);
	Vec3 center = bb_.center();
	setSceneCenter(qoglviewer::Vec(center[0], center[1], center[2]));
	showEntireScene();
}

void Viewer::addWidgets()
{
	checkBoxFlatLighting = new QCheckBox(tr("Flat lighting"));
	checkBoxShadowMapping = new QCheckBox(tr("Shadow mapping"));
	checkBoxSSAO = new QCheckBox(tr("SSAO"));
	checkBoxBorder = new QCheckBox(tr("Border"));
	checkBoxBlur = new QCheckBox(tr("Blur"));

	QVBoxLayout* checkBoxes = new QVBoxLayout;
	checkBoxes->addWidget(checkBoxFlatLighting);
	checkBoxes->addWidget(checkBoxShadowMapping);
	checkBoxes->addWidget(checkBoxSSAO);
	checkBoxes->addWidget(checkBoxBorder);
	checkBoxes->addWidget(checkBoxBlur);
	checkBoxes->addStretch(1);

	sliderRadiusSSAO = new QSlider();
	sliderRadiusSSAO->setMinimum(1);
	sliderRadiusSSAO->setMaximum(1000); 
	sliderRadiusSSAO->setValue(300);
	checkBoxes->addWidget(sliderRadiusSSAO);

	sliderRadiusBorder = new QSlider();
	sliderRadiusBorder->setMinimum(1);
	sliderRadiusBorder->setMaximum(100);
	sliderRadiusBorder->setValue(5);
	checkBoxes->addWidget(sliderRadiusBorder);

	radioButtonShowColor = new QRadioButton(tr("Color"));
	radioButtonShowNormal = new QRadioButton(tr("Normal"));

	checkBoxes->addWidget(radioButtonShowColor);
	checkBoxes->addWidget(radioButtonShowNormal);

	checkBoxFlatLighting->setChecked(false);
	checkBoxShadowMapping->setChecked(false);
	checkBoxSSAO->setChecked(false);

	radioButtonShowColor->setChecked(true); 
	setLayout(checkBoxes);

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);
	//layout()->addWidget(wdg);
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
	phong_rendering_(false),
	flat_rendering_(true),
	vertices_rendering_(false),
	edge_rendering_(false),
	normal_rendering_(false),
	bb_rendering_(true),
	pp_blur_(true)
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

void Viewer::renderModel(QMatrix4x4 proj, QMatrix4x4 mv, std::unique_ptr<cgogn::rendering::VBO>& vbo_pos, std::unique_ptr<cgogn::rendering::VBO>& vbo_norm)
{
	switch (renderMode)
	{
		case ShadowMap: 
			param_depth->set_position_vbo(vbo_pos.get());
			param_depth->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_depth->release();
			break; 

		case Gbuffer: 
			param_scene_data->set_vbos(vbo_pos.get(), vbo_norm.get());
			param_scene_data->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_scene_data->release();
			break;

		case Shadow:
			param_shadow->set_vbo(vbo_pos.get());
			param_shadow->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_shadow->release();
			break; 

		case SSAO:
			param_shadow->set_vbo(vbo_pos.get());
			param_shadow->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_shadow->release();
			break;

		case Border:
			param_border->set_vbo(vbo_pos.get());
			param_border->bind(proj.data(), mv.data());
			render_->draw(cgogn::rendering::TRIANGLES);
			param_border->release();
			break; 

		case Color:
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f, 2.0f);
			if (flat_rendering_)
			{
				param_flat_->bind(proj.data(), mv.data());
				param_flat_->set_enable_lighting(checkBoxFlatLighting->isChecked());
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
			break; 
	}
}

void Viewer::renderScene(QMatrix4x4 proj, QMatrix4x4 view, QMatrix4x4 shadowMVP)
{
	if (renderMode == Shadow)
	{
		texture_shadow_map->bindAt(0);
		texture_position->bindAt(1);
		texture_normal->bindAt(2);
		ssao_noiseTexture->bindAt(3);

		param_shadow->bind();
		param_shadow->set_enable_shadow(checkBoxShadowMapping->isChecked());
		param_shadow->set_enable_ssao(checkBoxSSAO->isChecked());
		param_shadow->set_shadowMap(texture_shadow_map->slot());
		param_shadow->set_shadowMVP(shadowMVP.data());
		param_shadow->set_pixelSize(1.0f / float(shadowMapResolution));
		param_shadow->set_radius_ssao(float(sliderRadiusSSAO->value())*0.1f);
		param_shadow->set_sampler_scene_position(texture_position->slot());
		param_shadow->set_sampler_scene_normal(texture_normal->slot());
		param_shadow->set_sampler_noise(ssao_noiseTexture->slot());
		param_shadow->set_noise_scale(cgogn::Vector2f(1.0f / float(noiseSize), 1.0f / float(noiseSize)));
		param_shadow->release();
	}
	else if (renderMode == Border)
	{
		param_border->bind();
		param_border->set_radius(float(sliderRadiusBorder->value())*0.01f);
		param_border->set_sampler_scene_position(texture_position->slot());
		param_border->set_sampler_scene_normal(texture_normal->slot());
		param_border->set_sampler_noise(ssao_noiseTexture->slot());
		param_border->set_noise_scale(cgogn::Vector2f(1.0f / float(noiseSize), 1.0f / float(noiseSize)));
		param_border->release();
	}
	
	switch (renderMode)
	{
		case ShadowMap: 
			param_depth->set_position_vbo(vbo_surface_pos_.get());
			param_depth->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_depth->release();
			break; 
		
		case Gbuffer:
			param_scene_data->set_vbos(vbo_surface_pos_.get(), vbo_surface_norm_.get());
			param_scene_data->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_scene_data->release();
			break; 
		
		case Shadow: 
			param_shadow->set_vbo(vbo_surface_pos_.get());
			param_shadow->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_shadow->release();
			break; 
		
		case SSAO: 
			param_shadow->set_vbo(vbo_surface_pos_.get());
			param_shadow->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_shadow->release();
			break; 
	
		case Border: 
			param_border->set_vbo(vbo_surface_pos_.get());
			param_border->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_border->release();
			break; 
		
		case Color: 
			param_simplecolor->set_position_vbo(vbo_surface_pos_.get());
			param_simplecolor->bind(proj.data(), view.data());
			buffer_surface_indices_->bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);;
			buffer_surface_indices_->release();
			param_simplecolor->release();
			break; 
	}
	
	float offset = 35.0f;
	QMatrix4x4 m = QMatrix4x4();
	
	if (useGargoyle)
	{
		m.setToIdentity();
		m.rotate(90.0f, QVector3D(1, 0, 0)); 
		m.translate(0.0f, 10.0f, 90.0f);
		m.scale(5.0f);
		auto mv = view * m;
		auto smv = shadowMVP * m;
		renderModel(proj, mv, vbo_pos_, vbo_norm_);
	}
	else
	{
		m.setToIdentity();
		m.translate(-5.0f*offset, -5.0f*offset, 0.0f);
		for (int x = 0; x < 11; x++)
		{
			for (int y = 0; y < 11; y++)
			{
				auto mv = view * m;
				auto smv = shadowMVP * m;
				renderModel(proj, mv, vbo_pos_, vbo_norm_);
				m.translate(0.0f, offset, 0.0f);
			}
			m.translate(offset, -11.0f*offset, 0.0f);
		}

		m.setToIdentity();
		//m.rotate(90.0f, QVector3D(1, 0, 0)); 

		m.translate(0.0f, 0.0f, 90.0f);
		m.scale(5.0f);
		auto mv = view * m;
		auto smv = shadowMVP * m;
		renderModel(proj, mv, vbo_pos_, vbo_norm_);
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

	float shadowFrustrum = 500; 
	QMatrix4x4 lightProj;
	lightProj.ortho(-shadowFrustrum, shadowFrustrum, -shadowFrustrum, shadowFrustrum, -shadowFrustrum, shadowFrustrum);

	QMatrix4x4 lightView;
	lightView.lookAt(QVector3D(10.0f, -10.0f, 10.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); 

	QMatrix4x4 lightVPbiased = QMatrix4x4(0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f) * lightProj * lightView;

	renderMode = ShadowMap; 
	fbo_shadow_map->bind();
	glViewport(0, 0, shadowMapResolution, shadowMapResolution);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(lightProj, lightView, QMatrix4x4());
	fbo_shadow_map->release();

	renderMode = Gbuffer;
	fbo_scene_data->bind();
	fbo_scene_data->drawBuffers(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1);
	glViewport(0, 0, width_, height_);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(proj, view, QMatrix4x4());
	fbo_scene_data->release();

	renderMode = Shadow;
	fbo_shadow->bind();
	fbo_shadow->drawBuffers(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, width_, height_);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(proj, view, lightVPbiased);
	fbo_shadow->release();

	renderMode = Border;
	fbo_border->bind();
	fbo_border->drawBuffers(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, width_, height_);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (checkBoxBorder->isChecked())
		renderScene(proj, view, QMatrix4x4());
	fbo_border->release();

	renderMode = Color; 
	fbo_color->bind();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(proj, view, QMatrix4x4());
	fbo_color->release();

	if (checkBoxBlur->isChecked())
	{
		//Pass blur 1
		fbo_blur1->bind();

		texture_light->bindAt(0);
		texture_position->bindAt(1);

		param_blur->bind();
		param_blur->set_blurred(texture_light->slot());
		param_blur->set_position_texture(texture_position->slot());
		param_blur->set_radius(float(sliderRadiusSSAO->value())*0.1f);
		param_blur->set_blur_dimension(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		param_blur->release();

		fbo_blur1->release();

		//Pass blur 2
		fbo_blur2->bind();

		texture_blur1->bindAt(0);
		texture_position->bindAt(1);

		param_blur->bind();
		param_blur->set_blurred(texture_blur1->slot());
		param_blur->set_position_texture(texture_position->slot());
		param_blur->set_radius(float(sliderRadiusSSAO->value())*0.1f);
		param_blur->set_blur_dimension(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		param_blur->release();

		fbo_blur2->release();

	}

	if (radioButtonShowColor->isChecked())
		texture_color->bindAt(0);
	else if (radioButtonShowNormal->isChecked())
		texture_normal->bindAt(0);
	texture_light->bindAt(1); 
	texture_border->bindAt(2);

	param_light_blend->bind();
	param_light_blend->set_sampler_color(0);
	param_light_blend->set_sampler_light(texture_light->slot());
	param_light_blend->set_sampler_border(texture_border->slot()); 
	glDrawArrays(GL_TRIANGLES, 0, 6);
	param_light_blend->release();
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
	cgogn::Vector3f baseOffset = cgogn::Vector3f(0.0f, 0.0f, -16.0f);
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

	vbo_surface_norm_ = cgogn::make_unique<cgogn::rendering::VBO>(3);
	vbo_surface_norm_->allocate(8, 3);
	cgogn::Vector3f normals[8] =
	{
		cgogn::Vector3f(0.0f,0.0f,1.0f),
		cgogn::Vector3f(0.0f,0.0f,1.0f),
		cgogn::Vector3f(0.0f,0.0f,1.0f),
		cgogn::Vector3f(0.0f,0.0f,1.0f),
		cgogn::Vector3f(0.0f,0.0f,-1.0f),
		cgogn::Vector3f(0.0f,0.0f,-1.0f),
		cgogn::Vector3f(0.0f,0.0f,-1.0f),
		cgogn::Vector3f(0.0f,0.0f,-1.0f),
	};
	vbo_surface_norm_->bind();
	vbo_surface_norm_->copy_data(0, 8 * sizeof(cgogn::Vector3f), normals);
	vbo_surface_norm_->release();

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

	param_blur = cgogn::rendering::shaders::Blur::Param::generate();
	param_shadow = cgogn::rendering::shaders::Shadow::Param::generate();
	param_border = cgogn::rendering::shaders::Border::Param::generate();
	param_scene_data = cgogn::rendering::shaders::SceneData::Param::generate();
	param_depth = cgogn::rendering::shaders::Depth::Param::generate();
	param_light_blend = cgogn::rendering::shaders::LightBlend::Param::generate();

	param_edge_ = cgogn::rendering::ShaderBoldLine::generate_param();
	param_edge_->set_position_vbo(vbo_pos_.get());
	param_edge_->color_ = cgogn::Color(255,255,0);
	param_edge_->width_= 2.5f;

	param_simplecolor = cgogn::rendering::ShaderSimpleColor::generate_param();
	param_simplecolor->color_ = cgogn::Color(128, 128, 128);

	param_flat_ = cgogn::rendering::ShaderFlat::generate_param();
	param_flat_->set_position_vbo(vbo_pos_.get());
	param_flat_->front_color_ = cgogn::Color(180, 180,180);
	param_flat_->back_color_ = cgogn::Color(0,0,200);
	param_flat_->ambiant_color_ = cgogn::Color(100, 100, 100);

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

	texture_shadow_map = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_shadow_map->bind();
	texture_shadow_map->setImage2D_simple(shadowMapResolution, shadowMapResolution, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
	texture_shadow_map->release();

	fbo_shadow_map = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_shadow_map->bind();
	fbo_shadow_map->attach(texture_shadow_map, GL_DEPTH_ATTACHMENT);
	fbo_shadow_map->check();
	fbo_shadow_map->release();

	texture_position = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_position->bind();
	texture_position->setImage2D_simple(w, h, GL_RGB32F, GL_RGB, GL_FLOAT);
	texture_position->release();

	texture_normal = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_normal->bind();
	texture_normal->setImage2D_simple(w, h, GL_RGB32F, GL_RGB, GL_FLOAT);
	texture_normal->release();

	texture_light = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_light->bind();
	texture_light->setImage2D_simple(w, h, GL_RED, GL_RED, GL_FLOAT);
	texture_light->release();

	texture_border = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_border->bind();
	texture_border->setImage2D_simple(w, h, GL_RED, GL_RED, GL_FLOAT);
	texture_border->release();

	texture_depth_temp = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_depth_temp->bind();
	texture_depth_temp->setImage2D_simple(w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	texture_depth_temp->release();

	fbo_shadow = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_shadow->bind();
	fbo_shadow->attach(texture_depth_temp, GL_DEPTH_ATTACHMENT);
	fbo_shadow->attach(texture_light, GL_COLOR_ATTACHMENT0);
	fbo_shadow->check();
	fbo_shadow->release();

	fbo_border = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_border->bind();
	fbo_border->attach(texture_depth_temp, GL_DEPTH_ATTACHMENT);
	fbo_border->attach(texture_border, GL_COLOR_ATTACHMENT0);
	fbo_border->check();
	fbo_border->release();

	fbo_scene_data = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_scene_data->bind();
	fbo_scene_data->attach(texture_depth_temp, GL_DEPTH_ATTACHMENT);
	fbo_scene_data->attach(texture_position, GL_COLOR_ATTACHMENT0);
	fbo_scene_data->attach(texture_normal, GL_COLOR_ATTACHMENT1);
	fbo_scene_data->check();
	fbo_scene_data->release();

	texture_color = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_color->bind();
	texture_color->setImage2D_simple(w, h, GL_RGBA, GL_RGBA, GL_FLOAT);
	texture_color->release();

	fbo_color = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_color->bind();
	fbo_color->attach(texture_depth_temp, GL_DEPTH_ATTACHMENT);
	fbo_color->attach(texture_color, GL_COLOR_ATTACHMENT0);
	fbo_color->check();
	fbo_color->release();

	texture_blur1 = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	texture_blur1->bind();
	texture_blur1->setImage2D_simple(w, h, GL_RED, GL_RED, GL_FLOAT);
	texture_blur1->release();

	fbo_blur1 = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_blur1->bind();
	fbo_blur1->attach(texture_blur1, GL_COLOR_ATTACHMENT0);
	fbo_blur1->check();
	fbo_blur1->release();

	fbo_blur2 = cgogn::make_unique<cgogn::rendering::ogl::Framebuffer>();
	fbo_blur2->bind();
	fbo_blur2->attach(texture_light, GL_COLOR_ATTACHMENT0);
	fbo_blur2->check();
	fbo_blur2->release();

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	std::vector<cgogn::Vector3f> ssaoNoise;
	for (unsigned int i = 0; i < (noiseSize*noiseSize); i++)
	{
		cgogn::Vector3f noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
		ssaoNoise.push_back(noise);
	}

	ssao_noiseTexture = cgogn::make_unique<cgogn::rendering::ogl::Texture>();
	ssao_noiseTexture->bind();
	ssao_noiseTexture->setImage2D_repeat(noiseSize, noiseSize, GL_RGB16F, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	ssao_noiseTexture->release();

	QOGLViewer::resizeGL(w, h);
}

int main(int argc, char** argv)
{
	std::string surface_mesh;
	if (argc < 2)
	{
		cgogn_log_info("simple_viewer") << "USAGE: " << argv[0] << " [filename]";
		surface_mesh = std::string(DEFAULT_MESH_PATH) + std::string(useGargoyle ? "off/gargoyle.off" : "off/aneurysm_3D.off");
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
	viewer.addWidgets();
	viewer.show();
	//QPushButton *btn1 = new QPushButton("btn1");

	// Run main loop.
	return application.exec();
}
