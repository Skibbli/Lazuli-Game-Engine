#include "stdafx.h"

#include "StoredModel.h"
#include "Shader.h"
#include "GameObject.h"
#include "MeshRenderer.h"

#include <iostream>


StoredModel::StoredModel()
{
	m_numModelParts = 0;
}

StoredModel::StoredModel(std::string _name, std::string _file)
{
	m_name = _name;
	m_file = _file;

	m_numModelParts = 0;
}

void StoredModel::AddModelPart(ModelPart _modelPart)
{
	m_modelParts.push_back(_modelPart);
	m_numModelParts++;
}

std::string StoredModel::GetName()
{
	return m_name;
}

int StoredModel::GetNumMeshes()
{
	return m_numModelParts;
}

std::vector<ModelPart>& StoredModel::GetModelParts()
{
	return m_modelParts;
}