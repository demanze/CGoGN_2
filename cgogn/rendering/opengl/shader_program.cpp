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

#include "cgogn/rendering/opengl/shader_program.h"

namespace cgogn
{
	namespace rendering
	{
		namespace ogl
		{
			std::vector<ShaderProgram*>* ShaderProgram::instances_ = nullptr;

			void ShaderProgram::register_instance(ShaderProgram* sh)
			{
				if (instances_ == nullptr)
				{
					instances_ = new std::vector<ShaderProgram*>;
					instances_->reserve(256);
				}

				auto it = std::find(instances_->begin(), instances_->end(), sh);
				if (it == instances_->end())
					instances_->push_back(sh);
			}


			ShaderProgram:: ShaderProgram()
			{
				handle = glCreateProgram();
			}

			void ShaderProgram::addShader(GLenum type, const GLchar* source)
			{
				GLuint shader = glCreateShader(type);
				glShaderSource(shader, 1, &source, nullptr);
				glCompileShader(shader);
				glAttachShader(handle, shader);

				GLint status = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
				{
					GLint logLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
					std::vector<GLchar> infoLog(logLength);
					glGetShaderInfoLog(shader, logLength, &logLength, &infoLog[0]);
					cgogn_log_info("shader compilation") << &infoLog[0];
					glDeleteShader(shader);
				}

				shaders.push_back(shader);
			}

			void ShaderProgram::addShaderFromFile(GLenum type, const char* filename)
			{
				std::string name = std::string(STRINGIFY(SHADERPATH)) + std::string("/shaders/source/") + std::string(filename); 
				std::ifstream file(name);
				if (!file)
				{
					cgogn_log_info(std::string("couldn't open shader file ") + name);
				}
				std::stringstream buffer;
				buffer << file.rdbuf();
				addShader(type, buffer.str().c_str());
			}

			void ShaderProgram::link()
			{
				glLinkProgram(handle);

				GLint status = 0;
				glGetProgramiv(handle, GL_LINK_STATUS, &status);
				if (status == GL_FALSE)
				{
					GLint logLength = 0;
					glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);

					std::vector<GLchar> infoLog(logLength);
					glGetProgramInfoLog(handle, logLength, &logLength, &infoLog[0]);
					cgogn_log_info("shader linking") << &infoLog[0];

					delete this;

					return;
				}
			}

			void ShaderProgram::bind()
			{
				glUseProgram(handle);
				Uniform::currentProgram = handle;
			}

			void ShaderProgram::release()
			{
				glUseProgram(0);
				Uniform::currentProgram = 0;
			}

			void ShaderProgram::get_matrices_uniforms()
			{
				unif_mv_matrix_ = "model_view_matrix";
				unif_projection_matrix_ = "projection_matrix";
				unif_normal_matrix_ = "normal_matrix";
			}

			void ShaderProgram::set_matrices(const Matrix4f& proj, const Matrix4f& mv)
			{
				if (unif_projection_matrix_.found())
					unif_projection_matrix_.set(proj);
				if (unif_mv_matrix_.found())
					unif_mv_matrix_.set(mv);

				if (unif_normal_matrix_.found())
				{
					Matrix3f normal_matrix = mv.topLeftCorner<3,3>().inverse().transpose();
					unif_normal_matrix_.set(normal_matrix);
				}
			}

			void ShaderProgram::set_view_matrix(const Matrix4f& mv)
			{
				unif_mv_matrix_.set(mv);

				if (unif_normal_matrix_.found())
				{
					Matrix3f normal_matrix = mv.topLeftCorner<3, 3>().inverse().transpose();
					unif_normal_matrix_.set(normal_matrix);
				}
			}

			void ShaderProgram::bindAttributeLocation(const GLchar* name, GLuint index)
			{
				glBindAttribLocation(handle, index, name);
			}

			ShaderProgram::~ShaderProgram()
			{

				for (GLuint shader : shaders)
				{
					glDeleteShader(shader);
				}
				glDeleteProgram(handle);
			}

			void ShaderProgram::clean_all()
			{
				if (instances_ != nullptr)
				{
					for (auto* ptr : *instances_)
						delete ptr;
					delete instances_;
					instances_ = nullptr;
				}
			}
		}
	}
}
