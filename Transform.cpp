#include "stdafx.h"

#include "Transform.h"
#include "GameObject.h"
#include "Maths.h"
#include "stdafx.h"

#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"

Transform::Transform()
{
	m_localPosition = Vec3(0,0,0);
	m_localScale = Vec3(1,1,1);
	m_localRotation = glm::quat(1,0,0,0);

	m_modelMat = Mat4(1.0f);
	m_manualMove = false;
}

Transform::~Transform() {}

// ++++++++++++++++++++++
// Position functions
// ++++++++++++++++++++++

Vec3 Transform::GetWorldPosition()
{
	Vec3 localPos(0.0f);
	weak<Transform> trans = m_parent;

	while(!trans.expired())
	{
		localPos += trans.lock()->GetLocalPosition();

		trans = trans.lock()->m_parent;
	}

	return localPos + m_localPosition;
}

Vec3 Transform::GetLocalPosition()
{
	return m_localPosition;
}

void Transform::SetWorldPosition(Vec3 _pos)
{
	weak<Transform> trans = m_parent;
	Vec3 localPos(0.0f);
	
	while(!trans.expired())
	{
		localPos += trans.lock()->GetLocalPosition();

		trans = trans.lock()->m_parent;
	}

	m_localPosition = _pos - localPos;
	m_modelMat = glm::translate(m_modelMat, _pos);

	m_manualMove = true;
}

void Transform::SetWorldPositionFromPhysics(Vec3 _pos)
{
	weak<Transform> trans = m_parent;
	Vec3 localPos(0.0f);
	
	while(!trans.expired())
	{
		localPos += trans.lock()->GetLocalPosition();

		trans = trans.lock()->m_parent;
	}

	m_modelMat = glm::translate(m_modelMat, _pos - m_localPosition);
	m_localPosition = _pos - localPos;
}

void Transform::SetLocalPosition(Vec3 _pos)
{
	m_modelMat = glm::translate(m_modelMat, (_pos - m_localPosition));
	m_localPosition = _pos;

	m_manualMove = true;
}

void Transform::Translate(Vec3 _translation)
{
	m_localPosition += _translation;
	m_modelMat = glm::translate(m_modelMat, _translation);

	m_manualMove = true;
}

void Transform::MoveForward(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(0,0,_dist));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}

void Transform::MoveBackward(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(0,0,-_dist));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}

void Transform::MoveLeft(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(-_dist,0,0));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}

void Transform::MoveRight(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(_dist,0,0));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}

void Transform::MoveUp(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(0,_dist,0));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}

void Transform::MoveDown(float _dist)
{
	m_modelMat = glm::translate(m_modelMat, Vec3(0,-_dist,0));
	m_localPosition = Vec3(m_modelMat[3][0], m_modelMat[3][1], m_modelMat[3][2]);

	m_manualMove = true;
}


// ++++++++++++++++++++++
// Rotation functions
// ++++++++++++++++++++++

Quat Transform::GetWorldRotation()
{
	weak<Transform> trans = m_parent;
	Quat rotation;

	rotation = m_localRotation;

	while(!trans.expired())
	{
		rotation *= trans.lock()->m_localRotation;
		trans = trans.lock()->m_parent;
	}

	return rotation;
}

Quat Transform::GetLocalRotation()
{
	return m_localRotation;
}

void Transform::SetWorldRotation(Quat _rot)
{
	Quat diff = _rot * glm::inverse(GetWorldRotation());

	m_localRotation *= diff;
	m_modelMat = GetTransMat(m_localPosition, m_localRotation, m_localScale);
	//m_modelMat *= glm::mat4_cast(diff);

	m_manualMove = true;
}

void Transform::SetWorldRotation(Vec3 _rot)
{
	Quat rot(_rot);

	Quat diff = rot * glm::inverse(GetWorldRotation());

	m_localRotation *= diff;
	m_modelMat = GetTransMat(m_localPosition, m_localRotation, m_localScale);

	m_manualMove = true;
}

void Transform::SetWorldRotationFromPhysics(btQuaternion &_rot)
{
	Quat glmRot;
	glmRot.w = _rot.w();
	glmRot.x = _rot.x();
	glmRot.y = _rot.y();
	glmRot.z = _rot.z();

	m_localRotation = glmRot;
	m_modelMat = GetTransMat(m_localPosition, m_localRotation, m_localScale);
}

void Transform::SetLocalRotation(Quat _rot)
{
	m_localRotation = _rot;

	m_manualMove = true;

	m_modelMat = GetTransMat(m_localPosition, _rot, m_localScale);
}

void Transform::Rotate(Quat _rot)
{
	m_localRotation = _rot * m_localRotation;

	Mat4 rotate = glm::mat4_cast(_rot);

	m_modelMat *= rotate;

	m_manualMove = true;
}

void Transform::Rotate(float _angle, Vec3 _axis)
{
	Quat quat;
	quat.w = cos(_angle / 2);
	quat.x = sin(_angle / 2) * _axis.x;
	quat.y = sin(_angle / 2) * _axis.y;
	quat.z = sin(_angle / 2) * _axis.z;

	m_localRotation *= quat;

	Mat4 rotate = glm::mat4_cast(quat);

	m_modelMat *= rotate;

	m_manualMove = true;
}

void Transform::RollLeft(float _angle)
{
	Quat rot(std::cos(_angle / 2), std::sin(_angle/2) * 0, std::sin(_angle/2) * 0, std::sin(_angle/2) * 1);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}

void Transform::RollRight(float _angle)
{
	Quat rot(std::cos(-_angle / 2), std::sin(-_angle/2) * 0, std::sin(-_angle/2) * 0, std::sin(-_angle/2) * 1);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}

void Transform::PitchUp(float _angle)
{
	Quat rot(std::cos(-_angle / 2), std::sin(-_angle/2) * 1, std::sin(-_angle/2) * 0, std::sin(-_angle/2) * 0);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}

void Transform::PitchDown(float _angle)
{
	Quat rot(std::cos(_angle / 2), std::sin(_angle/2) * 1, std::sin(_angle/2) * 0, std::sin(_angle/2) * 0);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}

void Transform::YawLeft(float _angle)
{
	Quat rot(std::cos(-_angle / 2), std::sin(-_angle/2) * 0, std::sin(-_angle/2) * 1, std::sin(-_angle/2) * 0);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}

void Transform::YawRight(float _angle)
{
	Quat rot(std::cos(_angle / 2), std::sin(_angle/2) * 0, std::sin(_angle/2) * 1, std::sin(_angle/2) * 0);

	Mat4 rotate = glm::mat4_cast(rot);

	m_modelMat *= rotate;

	m_localRotation *= rot;
	m_manualMove = true;
}


// ++++++++++++++++++++++
// Scale functions
// ++++++++++++++++++++++

Vec3 Transform::GetWorldScale()
{
	return Vec3(1);
}

Vec3 Transform::GetLocalScale()
{
	return m_localScale;
}

void Transform::SetLocalScale(Vec3 _scale)
{
	m_localScale = _scale;

	m_modelMat = glm::scale(m_modelMat, _scale);
}

void Transform::ChangeScale(float _scale)
{
	m_localScale *= _scale;
}

// ++++++++++++++++++++++
// Parent functions
// ++++++++++++++++++++++

void Transform::SetParent(weak<Transform> _transform)
{
	m_parent = _transform;
}

weak<Transform> Transform::GetParent()
{
	return m_parent;
}

void Transform::AddChild(weak<Transform> _transform)
{
	m_children.push_back(_transform);
}

Mat4 Transform::GetModelMatrix()
{
	Mat4 model;
	weak<Transform> trans = m_parent;

	while(!trans.expired())
	{
		model *= trans.lock()->m_modelMat;

		trans = trans.lock()->m_parent;
	}

	return m_modelMat * model;
}

bool Transform::ManuallyMoved()
{
	return m_manualMove;
}

void Transform::SetManMoved(bool _moved)
{
	m_manualMove = _moved;
}

// +++++++++++++++++++++++++++++++++++
// Inherited component functions
// +++++++++++++++++++++++++++++++++++

void Transform::Awake()
{

}

void Transform::Start()
{

}

void Transform::Destroy()
{

}