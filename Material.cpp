#include "stdafx.h"

#include "Material.h"
#include "Resources.h"


Material::Material()
{
	ResetMat();
}

Material::Material(std::string _name)
{
	ResetMat();

	m_name = _name;

	if(_name == "defaultMat")
	{
		m_matProperties.ambient = Vec3(0.0f);
		m_matProperties.diffuse = Vec3(0.3f);
		m_matProperties.emissive = Vec3(0.0f);
		m_matProperties.specular = Vec3(1.0f);
		m_matProperties.specPower = 50.0f;
	}

	else
	{
		try
		{
			m_diffuseTex = al_load_bitmap(("Assets/Models/" + m_name).c_str());
			m_hasTexture = true;
		}

		catch(...)
		{
			printf("Could not load texture for material: %s", _name);
		}
	}
}

Material::Material(aiMaterial* _mat, std::string _file)
{
	ResetMat();

	aiString str;
	if(_mat->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS)
	{
		m_name = str.C_Str();

		try
		{
			m_diffuseTex = al_load_bitmap(("Assets/Models/" + m_name).c_str());
			m_hasTexture = true;
		}

		catch(...)
		{
			printf("Could not load texture for material: %s", m_name);
		}		
	}
}

Material::~Material()
{
	///al_destroy_bitmap(m_tex);
}

std::string Material::GetName()
{
	return m_name;
}

void Material::BindTexture()
{
	m_shader.lock()->ActivateShader();
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexLoc);
}

void Material::ResetMat()
{
	m_matProperties.ambient = Vec3(0.0f);
	m_matProperties.diffuse = Vec3(0.0f);
	m_matProperties.emissive = Vec3(0.0f);
	m_matProperties.specular = Vec3(0.0f);
	m_matProperties.specPower = 0.0f;
	m_matProperties.refraction = 0.0f;
	m_matProperties.alpha = 0.0f;

	m_emissiveColourLoc = -1;
	m_ambientColourLoc = -1;
	m_diffuseColourLoc = -1;
	m_specularColourLoc = -1;
	m_specPowerLoc = -1;
	m_alphaLoc = -1;

	//m_ambientTexLoc = -1;
	m_diffuseTexLoc = -1;
	m_specularTexLoc = -1;
	/*m_specularHighlightLoc = -1;
	m_alphaTexLoc = -1;
	m_bumpTexLoc = -1;*/

	//m_ambientTex = nullptr;
	m_diffuseTex = nullptr;
	m_specularTex = nullptr;
	/*m_specularHighlight = nullptr;
	m_alphaTex = nullptr;
	m_bumpTex = nullptr;
	m_displacementTex = nullptr;
	m_stencilTex = nullptr;*/

	m_hasTexture = false;
}

bool Material::HasTexture()
{
	return m_hasTexture;
}

void Material::InitMaterial()
{
	LinkToShader();
}

void Material::LinkToShader()
{
	/// Link to variable locations in the shader
	m_ambientColourLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.ambient");
	m_diffuseColourLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.diffuse");
	m_emissiveColourLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.emissive");
	m_specularColourLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.specular");
	m_specPowerLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.specPower");
	m_refractionLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.refraction");
	m_alphaLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.properties.alpha");

	//m_ambientTexLoc = glGetUniformLocation(m_shader.GetShaderProgram(), "ambientTex1");
	m_diffuseTexLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.diffuse");
	m_specularTexLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "material.specular");
	/*m_specularHighlightLoc = glGetUniformLocation(m_shader.GetShaderProgram(), "specHighTex1");
	m_alphaTexLoc = glGetUniformLocation(m_shader.GetShaderProgram(), "alphaTex1");
	m_bumpTexLoc = glGetUniformLocation(m_shader.GetShaderProgram(), "bumpTex1");*/

	if(m_hasTexture)
		m_diffuseTexGL = al_get_opengl_texture(m_diffuseTex);

	m_hasTexLoc = glGetUniformLocation(m_shader.lock()->GetShaderProgram(), "hasTexture");
}

void Material::ActivateMaterial()
{
	m_shader.lock()->ActivateShader();

	//Set emissive and diffuse colours for the object
	glUniform3f(m_ambientColourLoc, m_matProperties.ambient.x, m_matProperties.ambient.y, m_matProperties.ambient.z);
	glUniform3f(m_diffuseColourLoc, m_matProperties.diffuse.x, m_matProperties.diffuse.y, m_matProperties.diffuse.z);
	glUniform3f(m_emissiveColourLoc, m_matProperties.emissive.x, m_matProperties.emissive.y, m_matProperties.emissive.z);
	glUniform3f(m_specularColourLoc, m_matProperties.specular.x, m_matProperties.specular.y, m_matProperties.specular.z);
	glUniform1f(m_specPowerLoc, m_matProperties.specPower);
	glUniform1f(m_refractionLoc, m_matProperties.refraction);
	glUniform1f(m_alphaLoc, m_matProperties.alpha);

	if(m_hasTexture)
	{
		glUniform1i(m_hasTexLoc, 1);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexGL);
	}

	else
	{
		glUniform1i(m_hasTexLoc, 0);
	}
}

void Material::SetShader(weak<Shader> _shader)
{
	m_shader = _shader;
}

GLint Material::GetShaderProgram()
{
	return m_shader.lock()->GetShaderProgram();
}