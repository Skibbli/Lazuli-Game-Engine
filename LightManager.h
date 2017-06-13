#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "stdafx.h"

#include "Light.h"

class LightManager
{
	public:
		LightManager();
		~LightManager();

		/// Add a directional light to the list of lights in the scene
		void AddDirLight(shared<DirLight>& _light);
		/// Add a point light to the list of lights in the scene
		void AddPointLight(shared<PointLight>& _light);
		/// Add a spot light to the list of lights in the scene
		void AddSpotLight(shared<SpotLight>& _light);

		/// Reloads the shaders while the engine is running. When shaders are reloaded the locations of variables in the shader need to be refound
		void ReloadShaders();

		/// Initialises the light manager
		void Initialise();
		void Awake();
		void Update();

	private:
		std::vector<weak<DirLight>> m_dirLights;
		std::vector<weak<PointLight>> m_pointLights;
		std::vector<weak<SpotLight>> m_spotLights;

		/// Number of directional lights in the scene
		int m_numDirLights;
		/// Number of point lights in the scene
		int m_numPointLights;
		/// Number of spot lights in the scene
		int m_numSpotLights;

		/// Locations of the numbers of each light in the shader
		GLint m_dirNumLoc, m_pointNumLoc, m_spotNumLoc;

		/// The shader the lights are linked to
		weak<Shader> shader;
};

#endif