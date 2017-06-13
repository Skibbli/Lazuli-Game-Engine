#include "stdafx.h"

#include "Collider.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "GameObject.h"


btVector3 GLMtoBT(Vec3 _vec)
{
	btVector3 vec;
	vec.setX(_vec.x);
	vec.setY(_vec.y);
	vec.setZ(_vec.z);

	return vec;
}

btQuaternion GLMtoBT(Quat _quat)
{
	btQuaternion quat;
	quat.setW(_quat.w);
	quat.setX(_quat.x);
	quat.setY(_quat.y);
	quat.setZ(_quat.z);

	return quat;
}

Collider::Collider()
{
	
}

Collider::~Collider()
{

}

void Collider::RecalculateBounds()
{
	weak<MeshRenderer> meshRenderer;
	weak<Mesh> mesh;

	meshRenderer = m_gameObject.lock()->GetComponent<MeshRenderer>();

	if(!meshRenderer.expired())
	{
		///mesh = meshRenderer.lock()->GetMesh();

		if(!mesh.expired())
		{
			mesh.lock()->RecalculateBounds();
			m_bounds = mesh.lock()->GetBounds();
		}
	}
}

bool Collider::CheckForCollision(weak<Collider> _coll)
{
	return false;
}

weak<Transform> Collider::GetTransform()
{
	return m_transform;
}

Bounds Collider::GetBounds()
{
	return m_bounds;
}

void Collider::ApplyForce(Vec3 _force)
{
	m_rigidBody->applyForce(GLMtoBT(_force), btVector3(0,0,0));
}

void Collider::SetLinearVelocity(int _axis, float _vel)
{
	btVector3 vel = m_rigidBody->getLinearVelocity();
	
	if(_axis == 0)
		vel.setX(_vel);

	if(_axis == 1)
		vel.setY(_vel);

	if(_axis == 2)
		vel.setZ(_vel);

	m_rigidBody->setLinearVelocity(vel);
	
}

void Collider::SetAngularVelocity(int _axis, float _vel)
{
	btVector3 vel = m_rigidBody->getAngularVelocity();
	
	if(_axis == 0)
		vel.setX(_vel);

	if(_axis == 1)
		vel.setY(_vel);

	if(_axis == 2)
		vel.setZ(_vel);

	m_rigidBody->setLinearVelocity(vel);
}