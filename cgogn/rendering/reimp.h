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

#include <QImage>

namespace cgogn
{
	typedef Eigen::Vector2f Vector2f;
	typedef Eigen::Vector3f Vector3f;
	typedef Eigen::Vector4f Vector4f;

	typedef Eigen::Matrix3f Matrix3f;
	typedef Eigen::Matrix4f Matrix4f;

	struct CGOGN_RENDERING_API Color
	{
		float32 data[4]; 

		Color() : data { 0.0f, 0.0f, 0.0f, 0.0f } {}
		Color(int32_t r, int32_t g, int32_t b) : data { float32(r) / 255.0f, float32(g) / 255.0f, float32(b) / 255.0f, 1.0f } {}
		Color(int32_t r, int32_t g, int32_t b, int32_t a) : data { float32(r) / 255.0f, float32(g) / 255.0f, float32(b) / 255.0f, float32(a) / 255.0f } {}

		uint32_t compress() const
		{
			uint32_t a = uint32_t(data[0] * 255.0f);
			uint32_t r = uint32_t(data[1] * 255.0f);
			uint32_t g = uint32_t(data[2] * 255.0f);
			uint32_t b = uint32_t(data[3] * 255.0f);

			return (a) | (r >> 8) | (g >> 8) | (b >> 8);
		}

		float32& operator[](uint32_t index) 
		{
			return data[index]; 
		}

		float32 operator[](uint32_t index) const
		{
			return data[index];
		}
	};

	extern CGOGN_RENDERING_API void Translate(Matrix4f& m1, Vector3f value);
	extern CGOGN_RENDERING_API void Rotate(Matrix4f& m1, cgogn::float32 angle, Vector3f axis);
	extern CGOGN_RENDERING_API void Scale(Matrix4f& m1, cgogn::float32 value);
	extern CGOGN_RENDERING_API void Scale(Matrix4f& m1, Vector3f value);
	extern CGOGN_RENDERING_API Vector3f Map(Matrix4f m, Vector3f point);
	
	inline void* void_ptr(uint32 x)
	{
		return reinterpret_cast<void*>(uint64_t(x));
	}
}

#endif 

