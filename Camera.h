#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"

#include "glm.hpp"
#include "Component.h"


class Camera : public Component
{
	public:
		Camera();
		~Camera();

		/// Returns the projection matrix of this camera
		glm::mat4 GetProjectionMatrix();
		/// Sets the projection matrix of this camera
		void SetProjectionMatrix(glm::mat4 &_projMat);
		/// Resets the projection matrix of this camera
		void ResetProjectionMatrix();

		void SendProjMatToGPU();

		/// Gets the cull mask
		int GetCullMask();
		/// Sets the cull mask
		void SetCullMask(int _mask);

		virtual void Awake();
		virtual void Update();
		virtual void Start();
		virtual void Destroy();

	private:
		bool m_orthographic;

		int m_cullMask;

		float m_fieldOfView;		
		float m_orthographicSize;
		float m_nearClipPlane;
		float m_farClipPlane;		

		glm::mat4 m_projectionMatrix;

		int m_shaderViewMatrixLocation;
		int m_shaderProjMatrixLocation;
};

#endif