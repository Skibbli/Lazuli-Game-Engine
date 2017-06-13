#include "stdafx.h"

#include "MeshRenderer.h"

#include "Core.h"
#include "Resources.h"
#include "Maths.h"


MeshRenderer::MeshRenderer()
{

}

MeshRenderer::MeshRenderer(ModelPart &_modelPart)
{
	m_mesh = _modelPart.mesh;
	m_material = _modelPart.material;
	m_name = _modelPart.name;
}

void MeshRenderer::Awake()
{

}

void MeshRenderer::Start()
{
	///Creating the model matrix
	m_modelMatrix = glm::mat4(1.0f);

	///Creating the view matrix - this is represents the camera's position and orientation
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0.0f, 0.0f));
	
	///Creating the projection matrix
	m_projMatrix = Core::GetCameras().at(0).lock()->GetProjectionMatrix();

	m_material.lock()->InitMaterial();
	
	///Linking variables in the program with the locations of the corresponding uniforms in the shaders
	m_shaderModelMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "modelMatrix");
	m_shaderViewMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "viewMatrix");
	m_shaderProjMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "projMatrix");

	m_viewPosLoc = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "viewPos");

	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	///Set initial rotation to 0
	angle = 0.0f;
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Destroy()
{

}

void MeshRenderer::Render()
{
	m_projMatrix = Core::GetCameras().at(0).lock()->GetProjectionMatrix();

	weak<Transform> camTransform = Core::GetCameras().at(0).lock()->GetGameObj().lock()->GetComponent<Transform>();

	///m_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 550.0f);

	glUniform3f(m_viewPosLoc, camTransform.lock()->GetWorldPosition().x, camTransform.lock()->GetWorldPosition().y, camTransform.lock()->GetWorldPosition().z);

	m_viewMatrix = camTransform.lock()->GetModelMatrix();

	Mat4 viewMat(1.0f);

	Vec3 tempVec = camTransform.lock()->GetWorldPosition();
	////tempVec.z *= -1;

	viewMat = glm::translate(viewMat, tempVec);

	Mat4 rot = glm::mat4_cast(camTransform.lock()->GetWorldRotation());

	viewMat *= rot;
	m_viewMatrix = viewMat;

	m_viewMatrix = glm::inverse(m_viewMatrix);

	m_modelMatrix = m_transform.lock()->GetModelMatrix();
		
	///Send model, view and projection matrices to OpenGL
	glUniformMatrix4fv(m_shaderModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	glUniformMatrix4fv(m_shaderViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
	glUniformMatrix4fv(m_shaderProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_projMatrix));
	
	/// come back when more than 1 texture sampler in shader
	///glActiveTexture(GL_TEXTURE0);
	///glUniform1i(m_shaderTexSamplerLoc, 0);

	m_material.lock()->ActivateMaterial();

	m_mesh.lock()->RenderMesh();
}

void MeshRenderer::ReloadShader()
{
	///Linking variables in the program with the locations of the corresponding uniforms in the shaders
	m_shaderModelMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "modelMatrix");
	m_shaderViewMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "viewMatrix");
	m_shaderProjMatrixLocation = glGetUniformLocation(m_material.lock()->GetShaderProgram(), "projMatrix");
}

weak<Mesh> MeshRenderer::GetMesh()
{
	return m_mesh;
}

weak<Material> MeshRenderer::GetMaterial()
{
	return m_material;
}

void MeshRenderer::SetMaterial(weak<Material> _mat)
{	
	m_material = _mat;
}

void MeshRenderer::SetShader(weak<Shader> _shader)
{
	m_material.lock()->SetShader(_shader);
}

void MeshRenderer::SetMesh(weak<Mesh> _mesh)
{
	m_mesh = _mesh;
}

void MeshRenderer::LoadModelPart(ModelPart &_modelPart)
{
	m_mesh = _modelPart.mesh;
	m_material = _modelPart.material;
	m_name = _modelPart.name;
	m_offset = _modelPart.offset;
}

MeshRenderer::~MeshRenderer()
{

}