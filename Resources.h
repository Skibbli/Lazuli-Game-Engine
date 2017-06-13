#ifndef RESOURCES_H
#define RESOURCES_H

#include "stdafx.h"

#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "StoredModel.h"
#include "AudioSample.h"


class Resources
{
	public:
		Resources();
		~Resources();

		/// Initialises the resource manager
		static void Initialise();
		static void Shutdown();

		/// Returns a weak pointer to the given shader
		static weak<Shader> LoadShader(std::string _name);
		/// Returns a weak pointer to the given shader
		static weak<Shader> LoadShaderFromFile(std::string _name);

		static void ReloadShader(std::string _name);

		/// Returns a weak pointer to the given material
		static weak<Material> LoadMaterial(std::string _name);
		/// Loads the named material
		static weak<Material> LoadMaterialFromFile(std::string _name);

		/// Returns a weak pointer to the given mesh
		weak<Mesh> GetMesh(std::string _name);

		/// Returns the given model
		static StoredModel LoadModel(std::string _model);
		static StoredModel LoadModelFromFile(std::string _file);
		static StoredModel LoadOBJFromFile(std::string _name);
		static void ParseMTL(std::string _name);
			
		static weak<AudioSample> LoadAudioSample(std::string _name);				

	private:
		static std::vector<shared<Shader>> m_shaders;
		static std::vector<shared<Material>> m_materials;
		static std::vector<shared<Mesh>> m_meshes;
		static std::vector<shared<AudioSample>> m_audioSamples;

		static std::vector<StoredModel> m_models;

		static Vec3 GetCenterOfMesh(std::vector<Vec3>& _verts);
};

#endif