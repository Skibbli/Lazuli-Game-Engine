#include "stdafx.h"

#include "Component.h"

Component::Component()
{
	m_isActive = true;
}

Component::~Component()
{
	m_gameObject.reset();
}

void Component::SetGameObject(weak<GameObject> _obj)
{
	m_gameObject = _obj;
}

weak<GameObject> Component::GetGameObj()
{
	return m_gameObject;
}