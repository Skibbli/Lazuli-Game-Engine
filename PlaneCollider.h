#ifndef PLANE_COLLIDER_H
#define PLANE_COLLIDER_H

#include "stdafx.h"

#include "Collider.h"


/// Implementation of the plane collider
class PlaneCollider : public Collider
{
	public:
		PlaneCollider();
		~PlaneCollider();

		/// Creates the plane collider with the given parameters
		void CreateCollider(Vec3 _plane, Vec3 _pos);

	private:
};

#endif