#include "stdafx.h"

#include "Camera.h"
#include "Core.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetProjectionMatrix()
{
	///return m_projectionMatrix;

	if(m_orthographic == true)
	{
		return glm::ortho(0, Screen::GetWidth(), 0, Screen::GetHeight(), 1, 5000);
	}
	else
	{
		return glm::perspective(45.0f, 1.0f, 0.1f, 5000.0f);
	}	
}

void Camera::SetProjectionMatrix(glm::mat4 &_projMat)
{
	m_projectionMatrix = _projMat;
}

void Camera::ResetProjectionMatrix()
{

}

void Camera::SendProjMatToGPU()
{
	glUniformMatrix4fv(m_shaderProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
}

int Camera::GetCullMask()
{
	return m_cullMask;
}

void Camera::SetCullMask(int _mask)
{
	m_cullMask = _mask;
}

void Camera::Awake()
{
	Core::GetCameras().push_back(std::dynamic_pointer_cast<Camera>(this->shared_from_this()));

	m_orthographic = false;
	m_fieldOfView = 80.0f;
	m_orthographicSize = 100;
	m_nearClipPlane = 0.01f;
	m_farClipPlane = 1200.0f;
	m_cullMask = 1 << 0;

	m_projectionMatrix = glm::perspective(45.0f, 800.0f/600.0f, 0.01f, 1200.0f);
}

void Camera::Start()
{
	
}

void Camera::Update()
{

}

void Camera::Destroy()
{

}