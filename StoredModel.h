#ifndef STORED_MODEL_H
#define STORED_MODEL_H

#include "stdafx.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing fla
#include "MeshRenderer.h"


struct ModelPart;

/// Contains a list of all of the parts that make up the model
class StoredModel
{
	friend class Resources;

	public:
		StoredModel();
		StoredModel(std::string _name, std::string _file);

		/// Adds another part to the model
		void AddModelPart(ModelPart _modelPart);

		int GetNumMeshes();

		/// Returns the model parts of the model
		std::vector<ModelPart>& GetModelParts();

		/// Returns the name of the model
		std::string GetName();

	private:
		/// List of the parts that make up the model
		std::vector<ModelPart> m_modelParts;

		/// Number of parts the model has
		int m_numModelParts;

		///The name of the model
		std::string m_name;
		/// The filepath of the model
		std::string m_file;
};

#endif