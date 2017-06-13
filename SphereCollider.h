#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "stdafx.h"

#include "Collider.h"


/// Implementation of the sphere colliders
class SphereCollider : public Collider
{
	public:
		SphereCollider();
		~SphereCollider();

		/// Creates the collider with the given parameters
		void CreateCollider(float _radius, Vec3 _pos, float _mass);
		/// Creates the collider based on the bounds of the mesh attached to the game object this collider is on
		void CreateColliderFromMesh(float _mass);

		void Awake();
		void Start();

	private:
		/// Radius of the sphere collider
		float m_radius;
		/// Mass of the collider
		float m_mass;
};

#endif