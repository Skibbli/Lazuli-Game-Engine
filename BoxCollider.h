#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "stdafx.h"

#include "Collider.h"


/// Implementation of the box collider
class BoxCollider : public Collider
{
	public:
		BoxCollider();
		~BoxCollider();

		/// Create the box collider with the given parameters
		void CreateCollider(float _xSize, float _ySize, float _zSize, float _mass);
		/// Create the box collider based on the size of the mesh attached to the game object this collider is on
		void CreateColliderFromMesh(float _mass);

		void Awake();
		void Start();

	private:
		/// Details about the box collider
		float m_xSize, m_ySize, m_zSize, m_mass;
};

#endif