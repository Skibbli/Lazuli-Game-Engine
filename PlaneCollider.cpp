#include "stdafx.h"

#include "PlaneCollider.h"
#include "Core.h"

PlaneCollider::PlaneCollider()
{

}

void PlaneCollider::CreateCollider(Vec3 _plane, Vec3 _pos)
{
	btCollisionShape* groundShape = new btStaticPlaneShape(GLMtoBT(_plane), 1);

	// Instantiate the ground. Its orientation is the identity, Bullet quaternions
	// are specified in x,y,z,w form. The position is 1 metre below the ground,
	// which compensates the 1m offset we had to put into the shape itself.
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(_pos)));

	// The first and last parameters of the following constructor are the mass and
	// inertia of the ground. Since the ground is static, we represent this by
	// filling these values with zeros. Bullet considers passing a mass of zero
	// equivalent to making a body with infinite mass - it is immovable.
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	m_rigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	//m_rigidBody->setUserPointer(m_transform.lock().get());

	Core::AddCollider(m_rigidBody);
}

PlaneCollider::~PlaneCollider()
{

}