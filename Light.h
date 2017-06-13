#ifndef LIGHT_H
#define LIGHT_H

#include "stdafx.h"

#include "Component.h"
#include "Resources.h"
#include "GameObject.h"

/// The base class that each type of light derives from
class Light : public Component
{
	public:
		Light();
		~Light();

		/// Sends the details of the light to the GPU
		virtual void SendDataToGPU();

		/// Sets the number of the type of light this is in the scene
		void SetLightNum(int _num);

	protected:
		/// The colour of the light
		Vec3 m_colour;

		/// Location of the colour of the light in the shader
		GLint m_colLoc;

		/// The number of the type of light this is in the scene
		int m_lightNum;

		/// The transform of the game object this light is attached to
		weak<Transform> m_transform;
};


class DirLight : public Light
{
	public:
		DirLight();
		~DirLight();

		void Awake();

		/// Sends the details of the light to the GPU
		void SendDataToGPU();
		/// Sets the parameters of the light
		void SetLightParams(Vec3 _col, Vec3 _dir);
		/// Links the light to the variables needed in the shader
		void LinkToShader();

	private:
		/// The direction of the light
		Vec3 m_lightDir;

		GLint m_dirLoc;
};


class PointLight : public Light
{
	public:
		PointLight();
		~PointLight();

		void Awake();
		
		/// Sends the details of the light to the GPU
		void SendDataToGPU();
		/// Sets the parameters of the light
		void SetLightParams(Vec3 _col, float _const, float _lin, float _quad);
		/// Links the light to the variables needed in the shader
		void LinkToShader();

	private:
		/// Variables for calculating the attenuation of the light
		float m_constant, m_linear, m_quadratic;

		GLint m_conLoc, m_linLoc, m_quadLoc;
		GLint m_posLoc;
};


class SpotLight : public Light
{
	public:
		SpotLight();
		~SpotLight();

		void Awake();

		/// Sends the details of the light to the GPU
		void SendDataToGPU();
		/// Sets the parameters of the light
		void SetLightParams(Vec3 _col, float _const, float _lin, float _quad, float _cutOff, float _outCutOff);
		/// Links the light to the variables needed in the shader
		void LinkToShader();

	private:
		/// Variables for calculating the attenuation of the light
		float m_constant, m_linear, m_quadratic;

		GLint m_conLoc, m_linLoc, m_quadLoc;

		float m_cutOff, m_outerCutOff;

		GLint m_cutLoc, m_outCutLoc;

		GLint m_posLoc, m_dirLoc;
};

#endif