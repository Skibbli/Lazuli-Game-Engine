#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "stdafx.h"

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"


class MeshRenderer : public Component
{
	public:
		MeshRenderer();
		MeshRenderer(ModelPart &_modelPart);
		~MeshRenderer();

		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void Render();
		virtual void Destroy();

		/// Returns the mesh attached to this mesh renderer
		weak<Mesh> GetMesh();

		weak<Material> GetMaterial();

		/// Sets the material that's used when rendering the mesh
		void SetMaterial(weak<Material> _mat);
		/// Sets the shader that's used when rendering the mesh
		void SetShader(weak<Shader> _shader);
		/// Sets the mesh that is renderer
		void SetMesh(weak<Mesh> _mesh);

		void LoadModelPart(ModelPart &_modelPart);

		void ReloadShader();

	private:
		///weak<Mesh> m_mesh;
		weak<Material> m_material;
		weak<Mesh> m_mesh;

		///Ints that are used to accessed the corresponding variables in the shader
		GLint m_shaderModelMatrixLocation;
		GLint m_shaderViewMatrixLocation;
		GLint m_shaderProjMatrixLocation;

		GLint m_viewPosLoc;

		////Matrices for the different spaces
		glm::mat4 m_modelMatrix, m_viewMatrix, m_projMatrix;

		///Variable to store the rotation of the object
		float angle;

		Vec3 m_offset;
		glm::mat3 m_matOffset;

		/// The transform this component is attached to
		weak<Transform> m_transform;

		/// The name of the model on this mesh renderer
		std::string m_name;
};

#endif