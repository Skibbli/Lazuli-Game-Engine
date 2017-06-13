#ifndef CAPSULE_COLLIDER_H
#define CAPSULE_COLLIDER_H

#include "stdafx.h"

#include "Collider.h"

class CapsuleCollider : public Collider
{
	public:
		CapsuleCollider();
		~CapsuleCollider();

		/// Creates a capsule collider with the given parameters
		void CreateCollider(float _radius, float _height, Vec3 _pos, float _mass);
		/// Creates a capsule collider based on the bounds of the mesh attached to the game object this component is on
		void CreateColliderFromMesh(float _mass);

	private:
		/// The radius of the cylinder
		float m_radius;
		/// The height of the cylinder
		float m_height;
		/// The mass of the cylinder
		float m_mass;
};

#endif