#include "stdafx.h"

#include "Light.h"
#include "Core.h"


Light::Light()
{
	
}

void Light::SendDataToGPU()
{

}

void Light::SetLightNum(int _num)
{
	m_lightNum = _num;
}

Light::~Light()
{

}

DirLight::DirLight()
{

}

void DirLight::Awake()
{
	Core::GetLightManager().AddDirLight(std::dynamic_pointer_cast<DirLight>(this->shared_from_this()));

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	LinkToShader();

	SendDataToGPU();
}

/// Finds the location of necessary variables in the shader
void DirLight::LinkToShader()
{
	weak<Shader> shader = Resources::LoadShader("monolith");

	std::string num = std::to_string( m_lightNum);
	std::string colLoc = "dirLights[" + num + "].colour";

	std::string dirLoc = "dirLights[" + num + "].direction";

	m_colLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), colLoc.c_str());

	m_dirLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), dirLoc.c_str());
}

/// Sends the parameters of the light to the gpu
void DirLight::SendDataToGPU()
{
	glUniform3f(m_colLoc, m_colour.x, m_colour.y, m_colour.z);

	glUniform3f(m_dirLoc, m_lightDir.x, m_lightDir.y, m_lightDir.z);
}

/// Sets the parameters of the light
void DirLight::SetLightParams(Vec3 _col, Vec3 _dir)
{
	m_colour = _col;

	m_lightDir = _dir;
}

DirLight::~DirLight()
{

}

PointLight::PointLight()
{

}

void PointLight::Awake()
{
	Core::GetLightManager().AddPointLight(std::dynamic_pointer_cast<PointLight>(this->shared_from_this()));

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	LinkToShader();

	SendDataToGPU();
}

/// Finds the location of necessary variables in the shader
void PointLight::LinkToShader()
{
	weak<Shader> shader = Resources::LoadShader("monolith");

	std::string num = std::to_string( m_lightNum);
	std::string colLoc = "pointLights[" + num + "].colour";
	
	std::string conLoc = "pointLights[" + num + "].constant";
	std::string linLoc = "pointLights[" + num + "].linear";
	std::string quadLoc = "pointLights[" + num + "].quadratic";

	std::string posLoc = "pointLights[" + num + "].position";

	m_colLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), colLoc.c_str());

	m_conLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), conLoc.c_str());
	m_linLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), linLoc.c_str());
	m_quadLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), quadLoc.c_str());

	m_posLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), posLoc.c_str());
}

/// Sends the parameters of the light to the gpu
void PointLight::SendDataToGPU()
{
	glUniform3f(m_colLoc, m_colour.x, m_colour.y, m_colour.z);

	glUniform1f(m_conLoc, m_constant);
	glUniform1f(m_linLoc, m_linear);
	glUniform1f(m_quadLoc, m_quadratic);

	glUniform3f(m_posLoc, m_transform.lock()->GetWorldPosition().x, m_transform.lock()->GetWorldPosition().y, m_transform.lock()->GetWorldPosition().z);
}

/// Sets the parameters of the light
void PointLight::SetLightParams(Vec3 _col, float _const, float _lin, float _quad)
{
	m_colour = _col;

	m_constant = _const;
	m_linear = _lin;
	m_quadratic = _quad;

	m_colLoc = -1;
}

PointLight::~PointLight()
{

}


SpotLight::SpotLight()
{

}

void SpotLight::Awake()
{
	Core::GetLightManager().AddSpotLight(std::dynamic_pointer_cast<SpotLight>(this->shared_from_this()));

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	LinkToShader();

	SendDataToGPU();
}

/// Finds the location of necessary variables in the shader
void SpotLight::LinkToShader()
{
	weak<Shader> shader = Resources::LoadShader("monolith");

	std::string num = std::to_string( m_lightNum);
	std::string colLoc = "spotLights[" + num + "].colour";
	
	std::string conLoc = "spotLights[" + num + "].constant";
	std::string linLoc = "spotLights[" + num + "].linear";
	std::string quadLoc = "spotLights[" + num + "].quadratic";

	std::string posLoc = "spotLights[" + num + "].position";
	std::string dirLoc = "spotLights[" + num + "].direction";

	m_colLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), colLoc.c_str());

	m_conLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), conLoc.c_str());
	m_linLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), linLoc.c_str());
	m_quadLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), quadLoc.c_str());

	m_posLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), posLoc.c_str());
	m_dirLoc = glGetUniformLocation(shader.lock()->GetShaderProgram(), dirLoc.c_str());
}

/// Sends the parameters of the light to the gpu
void SpotLight::SendDataToGPU()
{
	glUniform3f(m_colLoc, m_colour.x, m_colour.y, m_colour.z);

	glUniform1f(m_conLoc, m_constant);
	glUniform1f(m_linLoc, m_linear);
	glUniform1f(m_conLoc, m_quadratic);

	glUniform3f(m_posLoc, m_transform.lock()->GetWorldPosition().x, m_transform.lock()->GetWorldPosition().y, m_transform.lock()->GetWorldPosition().z);
	glUniform3f(m_dirLoc, m_transform.lock()->GetWorldRotation().x, m_transform.lock()->GetWorldRotation().y, m_transform.lock()->GetWorldRotation().z);
}

/// Sets the parameters of the light
void SpotLight::SetLightParams(Vec3 _col, float _const, float _lin, float _quad, float _cutOff, float _outCutOff)
{
	m_colour = _col;

	m_constant = _const;
	m_linear = _lin;
	m_quadratic = _quad;

	m_cutOff = _cutOff;
	m_outerCutOff = _outCutOff;

	m_colLoc = -1;
}

SpotLight::~SpotLight()
{

}