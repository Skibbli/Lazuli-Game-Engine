#include "stdafx.h"

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>	


glm::vec3 ConvertAssToGLM(aiVector3D _vec3)
{
	glm::vec3 vec;
	vec.x = _vec3.x;
	vec.y = _vec3.y;
	vec.z = _vec3.z;

	return vec;
}

glm::vec2 ConvertAssToGLM(aiVector2D _vec3)
{
	glm::vec2 vec;
	vec.x = _vec3.x;
	vec.y = _vec3.y;

	return vec;
}

Mesh::Mesh() {};

Mesh::Mesh(aiMesh* _mesh)
{
	std::vector<Vec3> tempVerts;
	std::vector<Vec3> tempNorms;
	std::vector<glm::vec2> tempUVs;

	for(Uint  v = 0; v < _mesh->mNumVertices; v++)
		tempVerts.push_back(ConvertAssToGLM(_mesh->mVertices[v]));

	for(Uint  v = 0; v < _mesh->mNumVertices; v++)
		tempNorms.push_back(ConvertAssToGLM(_mesh->mNormals[v]));

	if(_mesh->mTextureCoords[0])
	{
		for(Uint u = 0; u < _mesh->mNumVertices; u++)
		{
			glm::vec2 vec;
			vec.x = _mesh->mTextureCoords[0][u].x;
			vec.y = _mesh->mTextureCoords[0][u].y;

			tempUVs.push_back(vec);
		}
	}
			
	for(Uint f = 0; f < _mesh->mNumFaces; f++)
	{
		aiFace face = _mesh->mFaces[f];
		for(Uint  g = 0; g < face.mNumIndices; g++)
		{
			m_vertices.push_back(tempVerts.at(face.mIndices[g]));
			m_normals.push_back(tempNorms.at(face.mIndices[g]));
			m_texCoords.push_back(tempUVs.at(face.mIndices[g]));
		}
	}

	SetupMesh();
	RecalculateBounds();

	m_name = _mesh->mName.C_Str();
}

Mesh::Mesh(std::vector<Vec3>& _verts, std::vector<Vec3>& _normals, std::vector<glm::vec2>& _texCoords, std::vector<VertexIndex>& _indices, std::string _name)
{
	m_name = _name;

	for(Uint i = 0; i < _indices.size(); i++)
	{
		m_vertices.push_back(_verts.at(_indices.at(i).vertIndex - 1));
		m_normals.push_back(_normals.at(_indices.at(i).normIndex - 1));
		m_texCoords.push_back(_texCoords.at(_indices.at(i).texIndex - 1));
	}
	
	RecalculateBounds();
}

Mesh::~Mesh()
{

}

std::vector<Vec3>& Mesh::GetVertices()
{
	return m_vertices;
}

Bounds Mesh::GetBounds()
{
	return m_bounds;
}

void Mesh::AdjustVertices(Vec3 _offset)
{
	for(Uint i = 0; i < m_vertices.size(); i++)
	{
		m_vertices.at(i) -= _offset;
	}

	RecalculateBounds();
	SetupMesh();
}

void Mesh::RecalculateBounds()
{
	float minX = m_vertices.at(0).x;
	float maxX = m_vertices.at(0).x;
	float minY = m_vertices.at(0).y;
	float maxY = m_vertices.at(0).y;
	float minZ = m_vertices.at(0).z;
	float maxZ = m_vertices.at(0).z;

	for(Uint i = 0; i < m_vertices.size(); i++)
	{
		if(m_vertices.at(i).x < minX)
			minX = m_vertices.at(i).x;

		if(m_vertices.at(i).x > maxX)
			maxX = m_vertices.at(i).x;

		if(m_vertices.at(i).y < minY)
			minY = m_vertices.at(i).y;

		if(m_vertices.at(i).y > maxY)
			maxY = m_vertices.at(i).y;

		if(m_vertices.at(i).z < minZ)
			minZ = m_vertices.at(i).z;

		if(m_vertices.at(i).z > maxZ)
			maxZ = m_vertices.at(i).z;
	}

	float midX = (maxX + minX) /2.0f;
	float midY = (maxY + minY) /2.0f;
	float midZ = (maxZ + minZ) /2.0f;

	float sizeX = maxX - minX;
	float sizeY = maxY - minY;
	float sizeZ = maxZ - minZ;

	m_bounds = Bounds(Vec3(midX, midY, midZ), Vec3(sizeX, sizeY, sizeZ), Vec3(1.0f,1.0f,1.0f));
}

void Mesh::RecalculateNormals()
{

}

std::string Mesh::GetName()
{
	return m_name;
}

void Mesh::RenderMesh()
{
	glBindVertexArray(m_VAO);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertBuffID);
	glGenBuffers(1, &m_normBuffID);
	glGenBuffers(1, &m_texBuffID);

	glBindVertexArray(m_VAO);

	///Bind vert buffer and fill it
    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), &m_vertices[0],  GL_STATIC_DRAW);

	///Tells the GPU some details about the data loaded and enables the array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); 

    ///Bind UV VBO and fill it with UV data
	glBindBuffer(GL_ARRAY_BUFFER, m_texBuffID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_texCoords.size(), &m_texCoords[0],  GL_STATIC_DRAW);

	///Tells the GPU some details about the data loaded and enables the array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	///Vind norm buffer and fill it with normal data
	glBindBuffer(GL_ARRAY_BUFFER, m_normBuffID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_normals.size(), &m_normals[0],  GL_STATIC_DRAW);

	///Tells the GPU some details about the data loaded and enables the array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
		
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/// Technically we can do this, because the enabled / disabled state is stored in the VAO
	glDisableVertexAttribArray(0);
}

//void Mesh::ResetOriginToCenter(Vec3 _offset)
//{
//	for(int i = 0; i < m_vertices.size(); i++)
//	{
//		m_vertices.at(i) -= _offset;
//	}
//}

Vec3 Mesh::GetCenter()
{
	return m_bounds.m_center;
}