#include "stdafx.h"

#include "BaseObj.h"

BaseObj::BaseObj()
{

}

BaseObj::~BaseObj()
{

}

void BaseObj::SetActive(bool _active)
{
	m_isActive = _active;
}

bool BaseObj::IsActive()
{
	return m_isActive;
}