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

#include "cgogn/rendering/reimp.h"

Vector4f Color(int r, int g, int b)
{
	return Vector4f(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 1.0f);
}

Vector4f Color(int r, int g, int b, int a)
{
	return Vector4f(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f);
}

uint32_t RGBA(Vector4f color)
{
	uint32_t a = uint32_t(color.w() * 255.0f);
	uint32_t r = uint32_t(color.x() * 255.0f);
	uint32_t g = uint32_t(color.y() * 255.0f);
	uint32_t b = uint32_t(color.z() * 255.0f);

	return (a) | (r >> 8) | (g >> 8) | (b >> 8);
}

void Translate(Matrix4f& m1, Vector3f value)
{
	Eigen::Affine3f transform(Eigen::Translation3f(value.x(), value.y(), value.z()));
	m1 = transform.matrix() * m1;
}

void Rotate(Matrix4f& m1, cgogn::float32 angle, Vector3f axis)
{
	Eigen::Affine3f transform(Eigen::AngleAxis<float>(angle, axis));
	m1 = transform.matrix() * m1;
}

void Scale(Matrix4f& m1, cgogn::float32 value)
{
	Eigen::Affine3f transform(Eigen::Scaling(value));
	m1 = transform.matrix() * m1;
}

void Scale(Matrix4f& m1, Vector3f value)
{
	Eigen::Affine3f transform(Eigen::Scaling(value));
	m1 = transform.matrix() * m1;
}

Vector3f Map(Matrix4f m, Vector3f point)
{
	Vector4f product = m * Vector4f(point.x(), point.y(), point.z(), 1.0f);
	return product.head<3>() / product.w();
}
