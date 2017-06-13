#ifndef MESH_COLLIDER_H
#define MESH_COLLIDER_H

#include "stdafx.h"

#include "Collider.h"
#include "Mesh.h"
#include "CustomMotionState.h"


/// Implementation of the mesh collider
class MeshCollider : Collider
{
	public:
		MeshCollider();
		~MeshCollider();

		/// Creates a mesh collider using the given parameter
		void CreateCollider(weak<Mesh> _mesh);

		/// Sets the mesh this collider is assigned to
		void SetMesh(weak<Mesh> _mesh);

	private:
		/// The mesh this collider is representing
		weak<Mesh> m_mesh;
};

#endif