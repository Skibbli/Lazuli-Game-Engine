#ifndef CUSTOM_MOTION_STATE_H
#define CUSTOM_MOTION_STATE_H

#include "stdafx.h"

#include <btBulletCollisionCommon.h>
#include "Transform.h"


/// Custom motion state that links to the engine's transform for the physics to update the objects
class CustomMotionState : public btMotionState
{
	public:
		CustomMotionState();
		/// Creates the motion state using the given parameters
		CustomMotionState(const btTransform &_initialPosition, weak<Transform> _trans);

		virtual ~CustomMotionState();

		/// Sets the transform this motion state is attached to
		void SetNode(weak<Transform> _trans);
		/// Gets the world transform of the collider this motion state is linked to
		virtual void getWorldTransform(btTransform &worldTrans) const;
		/// Sets the world transform of the collider this motion state is linked to
		virtual void setWorldTransform(const btTransform &worldTrans);

	protected:
		/// The transform of the object the collider is attached to
		weak<Transform> m_transform;
		/// The bullet transform of the collider this motion state is linked to
		btTransform m_position;
};

#endif