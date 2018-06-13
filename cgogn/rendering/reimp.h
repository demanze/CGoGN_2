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

#ifndef CGOGN_RENDERING_REIMP_H_
#define CGOGN_RENDERING_REIMP_H_

#include "cgogn/rendering/dll.h"
#include "cgogn/core/utils/logger.h"
#include "cgogn/rendering/opengl/glew.h"

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>

#include <vector>
#include <memory>

typedef Eigen::Vector2f Vector2f;
typedef Eigen::Vector3f Vector3f;
typedef Eigen::Vector4f Vector4f;

typedef Eigen::Matrix3f Matrix3f;
typedef Eigen::Matrix4f Matrix4f;

extern CGOGN_RENDERING_API Vector4f Color(int32_t r, int32_t g, int32_t b);
extern CGOGN_RENDERING_API Vector4f Color(int32_t r, int32_t g, int32_t b, int32_t a);
extern CGOGN_RENDERING_API uint32_t RGBA(Vector4f color);
extern CGOGN_RENDERING_API void Translate(Matrix4f& m1, Vector3f value);
extern CGOGN_RENDERING_API void Rotate(Matrix4f& m1, cgogn::float32 angle, Vector3f axis);
extern CGOGN_RENDERING_API void Scale(Matrix4f& m1, cgogn::float32 value);
extern CGOGN_RENDERING_API void Scale(Matrix4f& m1, Vector3f value);
extern CGOGN_RENDERING_API Vector3f Map(Matrix4f m, Vector3f point);

#endif 

