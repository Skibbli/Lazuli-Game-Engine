#include "stdafx.h"

#include "SphereCollider.h"
#include "GameObject.h"
#include "Core.h"
#include "CustomMotionState.h"


SphereCollider::SphereCollider()
{

}

void  SphereCollider::CreateCollider(float _radius, Vec3 _pos, float _mass)
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btCollisionShape* sphereShape = new btSphereShape(_radius);

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition())), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		sphereShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, sphereShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}

void SphereCollider::CreateColliderFromMesh(float _mass)
{
	RecalculateBounds();

	float radius;

	radius = m_bounds.m_size.x;

	if(radius < m_bounds.m_size.y)
		radius = m_bounds.m_size.y;

	if(radius < m_bounds.m_size.z)
		radius = m_bounds.m_size.z;


	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	btCollisionShape* sphereShape = new btSphereShape(radius);

	btMotionState *motionState = new CustomMotionState(btTransform(btQuaternion(0, 0, 0, 1), GLMtoBT(m_transform.lock()->GetWorldPosition() - m_bounds.m_center)), m_transform);

	btVector3 fallInertia(0, 0, 0);

	if(_mass >0.0f)
		sphereShape->calculateLocalInertia(_mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
		rigidBodyCI(_mass, motionState, sphereShape, fallInertia);

	m_rigidBody = std::make_shared<btRigidBody>(rigidBodyCI);

	m_rigidBody->setRestitution(0.1f);
	m_rigidBody->setFriction(0.5f);
	m_rigidBody->setUserPointer(m_transform.lock().get());

	m_mass = _mass;

	Core::AddCollider(m_rigidBody);
}

void SphereCollider::Awake()
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();
}

void SphereCollider::Start()
{

}

SphereCollider::~SphereCollider()
{

}