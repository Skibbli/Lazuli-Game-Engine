#include "stdafx.h"

#include "LightManager.h"
#include "Resources.h"

LightManager::LightManager()
{
	m_numDirLights = 0;
	m_numPointLights = 0;
	m_numSpotLights = 0;
}

void LightManager::AddDirLight(shared<DirLight>& _light)
{
	_light->SetLightNum(m_numDirLights);
	m_dirLights.push_back(_light);

	m_numDirLights++;
}

void LightManager::AddPointLight(shared<PointLight>& _light)
{
	_light->SetLightNum(m_numDirLights);
	m_pointLights.push_back(_light);

	m_numPointLights++;
}

void LightManager::AddSpotLight(shared<SpotLight>& _light)
{
	_light->SetLightNum(m_numDirLights);
	m_spotLights.push_back(_light);

	m_numSpotLights++;
}

void LightManager::Initialise()
{
	shader = Resources::LoadShader("monolith");
	m_dirNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numDirLights");
	m_pointNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numPointLights");
	m_spotNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numSpotLights");
}

void LightManager::ReloadShaders()
{
	shader = Resources::LoadShader("monolith");

	shader.lock()->ActivateShader();

	m_dirNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numDirLights");
	m_pointNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numPointLights");
	m_spotNumLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), "numSpotLights");

	glUniform1i(m_dirNumLoc, m_numDirLights);
	glUniform1i(m_pointNumLoc, m_numPointLights);
	glUniform1i(m_spotNumLoc, m_numSpotLights);

	for(weak<DirLight> light : m_dirLights)
	{
		light.lock()->LinkToShader();
	}

	for(weak<PointLight> light : m_pointLights)
	{
		light.lock()->LinkToShader();
	}

	for(weak<SpotLight> light : m_spotLights)
	{
		light.lock()->LinkToShader();
	}
}

void LightManager::Awake()
{
	shader.lock()->ActivateShader();

	glUniform1i(m_dirNumLoc, m_numDirLights);
	glUniform1i(m_pointNumLoc, m_numPointLights);
	glUniform1i(m_spotNumLoc, m_numSpotLights);
}

void LightManager::Update()
{
	for(Uint i = 0; i < m_dirLights.size(); i++)
	{
		m_dirLights.at(i).lock()->SendDataToGPU();
	}

	for(Uint i = 0; i < m_pointLights.size(); i++)
	{
		m_pointLights.at(i).lock()->SendDataToGPU();
	}

	for(Uint i = 0; i < m_spotLights.size(); i++)
	{
		m_spotLights.at(i).lock()->SendDataToGPU();
	}
}

LightManager::~LightManager()
{

}