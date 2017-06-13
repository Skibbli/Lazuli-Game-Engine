#ifndef BOUNDS_H
#define BOUNDS_H

#include "stdafx.h"

#include "glm.hpp"

#define Vec3 glm::vec3

class Bounds
{
	public:
		Bounds();
		Bounds(Vec3 _center, Vec3 _size, Vec3 _scale);
		~Bounds();

		/// Center of the model
		Vec3 m_center;
		/// Size of the model (furthest points apart)
		Vec3 m_size;
		/// Scale of the model
		Vec3 m_scale;

		/// Sets the scale of the bounds
		void SetScale(Vec3 _scale);

	private:
		
};

#endif