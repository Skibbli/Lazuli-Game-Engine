#include "stdafx.h"

#include "Bounds.h"

Bounds::Bounds()
{

}

Bounds::Bounds(Vec3 _center, Vec3 _size, Vec3 _scale)
{
	m_center = _center;
	m_size = _size;
	m_scale = _scale;
}

void Bounds::SetScale(Vec3 _scale)
{
	m_scale = _scale;
}

Bounds::~Bounds()
{

}