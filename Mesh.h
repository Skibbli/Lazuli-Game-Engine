#ifndef MESH_H
#define MESH_H

#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <Assimp/scene.h>
#include "Bounds.h"

/// Converts an Assimp Vector3D to a glm Vec3
glm::vec3 ConvertAssToGLM(aiVector3D _vec3);
/// Converts an Assimp Vector2D to a glm Vec2
glm::vec2 ConvertAssToGLM(aiVector2D _vec3);

class Mesh
{
	public:
		Mesh();
		Mesh(aiMesh* _mesh);
		Mesh(std::vector<Vec3>& _verts, std::vector<Vec3>& _normals, std::vector<glm::vec2>& _texCoords, std::vector<VertexIndex>& _indices, std::string _name);
		~Mesh();

		/// Returns whether the mesh has UVs
		bool HasUVs();
		/// Returns whether the mesh has normals
		bool HasNormals();

		/// Returns the bounds of the mesh
		Bounds GetBounds();

		/// Recalculates the bounds of the mesh
		void RecalculateBounds();
		/// Recalculates the normals of the mesh
		void RecalculateNormals();

		//void ResetOriginToCenter(Vec3 _offset);

		/// Returns the name of the mesh
		std::string GetName();

		/// Renders the mesh
		void RenderMesh();

		/// Adjusts the vertices of the mesh by the given amount
		void AdjustVertices(Vec3 _offset);
		/// Sets up the VAO and sends the data to the GPU
		void SetupMesh();
		/// Gets the center of the mesh
		Vec3 GetCenter();

		/// Gets the vertices of the mesh
		std::vector<Vec3>& GetVertices();

	private:
		///Place in one VBO in future
		GLuint m_VAO, m_vertBuffID, m_normBuffID, m_texBuffID;

		/// Vertices of the mesh
		std::vector<glm::vec3> m_vertices;
		/// Normals of the mesh
		std::vector<glm::vec3> m_normals;
		/// UV coordinates of the mesh
		std::vector<glm::vec2> m_texCoords;

		/// The name of the mesh
		std::string m_name;

		/// Details about the bounds of the mesh (size, center etc)
		Bounds m_bounds;
		Vec3 m_offset;
};

#endif