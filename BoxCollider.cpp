#include "stdafx.h"

#include "BoxCollider.h"
#include "GameObject.h"
#include "Core.h"
#include "CustomMotionState.h"


BoxCollider::BoxCollider()
{

}

void BoxCollider::CreateCollider(float _xSize, float _ySize, float _zSize, float _mass)
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btCollisionShape* boxShape = new btBoxShape(btVector3(_xSize, _ySize, _zSize));

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition())), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		boxShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, boxShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_rigidBody->setActivationState(DISABLE_DEACTIVATION);

	m_xSize = _xSize;
	m_ySize = _ySize;
	m_zSize = _zSize;
	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}

void BoxCollider::CreateColliderFromMesh(float _mass)
{
	RecalculateBounds();

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btCollisionShape* boxShape = new btBoxShape(btVector3(m_bounds.m_size.x, m_bounds.m_size.y, m_bounds.m_size.z));

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition() - m_bounds.m_center)), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		boxShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, boxShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_xSize = m_bounds.m_size.x;
	m_ySize = m_bounds.m_size.y;
	m_zSize = m_bounds.m_size.z;
	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}

void BoxCollider::Awake()
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	RecalculateBounds();
}

void BoxCollider::Start()
{

}

BoxCollider::~BoxCollider()
{

}