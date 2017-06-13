#ifndef COLLIDER_H
#define COLLIDER_H

#include "stdafx.h"

#include "Component.h"
#include "Bounds.h"
#include "Transform.h"

/// Converts a GLM Vec3 to a btVector3
btVector3 GLMtoBT(Vec3 _vec);
/// Converts a GLM Quaternion to a btQuaternion
btQuaternion GLMtoBT(Quat _quat);

class Transform;

/// The base class for the different colliders in the engine
class Collider : public Component
{
	public:
		Collider();
		~Collider();

		virtual bool CheckForCollision(weak<Collider> _coll);

		/// Returns the transform of the game object this collider is attached to
		weak<Transform> GetTransform();

		/// Recalculates the bounds of this collider
		void RecalculateBounds();
		/// Gets the bounds of this collider
		Bounds GetBounds();

		void ApplyForce(Vec3 _force);
		void SetLinearVelocity(int _axis, float _vel);
		void SetAngularVelocity(int _axis, float _vel);

	protected:
		/// The bounds of the mesh this collider is attached to
		Bounds m_bounds;
		/// The transform this collider is attached to, to speed up position getting/setting
		weak<Transform> m_transform;
		/// The rigidbody this collider is attached to
		shared<btRigidBody> m_rigidBody;
};

#endif