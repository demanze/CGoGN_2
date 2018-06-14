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

namespace cgogn
{
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
}