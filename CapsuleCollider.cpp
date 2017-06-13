#include "stdafx.h"

#include "CapsuleCollider.h"
#include "GameObject.h"
#include "Core.h"
#include "CustomMotionState.h"

CapsuleCollider::CapsuleCollider()
{

}

CapsuleCollider::~CapsuleCollider()
{

}

void CapsuleCollider::CreateCollider(float _radius, float _height, Vec3 _pos, float _mass)
{
	btCollisionShape* capShape = new btCapsuleShape(_radius, _height);

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition())), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		capShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, capShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}

void CapsuleCollider::CreateColliderFromMesh(float _mass)
{
	RecalculateBounds();

	float radius = m_bounds.m_size.x;

	if(radius < m_bounds.m_size.z)
		radius = m_bounds.m_size.z;

	btCollisionShape* capShape = new btCapsuleShape(radius, m_bounds.m_size.y);

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition() - m_bounds.m_center)), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		capShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, capShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}