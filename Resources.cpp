#include "stdafx.h"

#include "Resources.h"
#include "GameObject.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




//Static variables
std::vector<shared<Shader>> Resources::m_shaders;
std::vector<shared<Material>> Resources::m_materials;
std::vector<shared<Mesh>> Resources::m_meshes;
std::vector<shared<AudioSample>> Resources::m_audioSamples;

std::vector<StoredModel> Resources::m_models;

Resources::Resources() {};
Resources::~Resources() {};

void Resources::Initialise()
{
	al_reserve_samples(30);

	shared<Material> defMat = std::make_shared<Material>("defaultMat");
	defMat->m_shader = LoadShader("monolith");

	shared<AudioSample> sample = std::make_shared<AudioSample>("Background_Music.ogg");
	m_audioSamples.push_back(sample);

	m_materials.push_back(defMat);
}

void Resources::Shutdown()
{
	m_audioSamples.clear();
}

StoredModel Resources::LoadModelFromFile(std::string _file)
{
	std::string filepath = "Assets/Models/";

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile((filepath +_file), aiProcess_Triangulate);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

	StoredModel model(_file.substr(0, _file.find_last_of('/')), _file);

	int materialOffset = m_materials.size();

	if(scene->mNumMaterials - 1 > 0)
	{
		for(Uint i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];

			shared<Material> tempMat = std::make_shared<Material>(material, _file);
			tempMat->m_shader = LoadShader("monolith");
			m_materials.push_back(tempMat);
		}
	}

	for(Uint  i = 0; i < scene->mNumMeshes; i++)
	{
		ModelPart modelPart;

		aiMesh* mesh = scene->mMeshes[i];

		shared<Mesh> tempMesh = std::make_shared<Mesh>(mesh);

		m_meshes.push_back(tempMesh);

		modelPart.mesh = tempMesh;

		if(scene->mNumMaterials - 1 > 0)
		{
			modelPart.material = m_materials.at(mesh->mMaterialIndex + materialOffset);
		}

		else
		{
			modelPart.material = LoadMaterial("defaultMat");
		}

		modelPart.name = mesh->mName.C_Str();

		model.AddModelPart(modelPart);
	}

	m_models.push_back(model);
	
	return model;
}

weak<AudioSample> Resources::LoadAudioSample(std::string _name)
{
	for(Uint i = 0; i < m_audioSamples.size(); i++)
	{
		if(m_audioSamples.at(i)->GetName() == _name)
			return m_audioSamples.at(i);
	}

	shared<AudioSample> sample = std::make_shared<AudioSample>(_name);
	m_audioSamples.push_back(sample);

	return sample;
}

weak<Shader> Resources::LoadShader(std::string _name)
{
	bool found = false;
	
	for(Uint i = 0; i < m_shaders.size(); i++)
	{
		if(m_shaders.at(i)->GetName() == _name)
			return m_shaders.at(i);
	}

	if(!found)
	{
		return LoadShaderFromFile(_name);
	}
}

weak<Shader> Resources::LoadShaderFromFile(std::string _name)
{
	shared<Shader> shader = std::make_shared<Shader>(_name);

	m_shaders.push_back(shader);
	return shader;
}

weak<Material> Resources::LoadMaterial(std::string _name)
{
	shared<Material> material;

	for(shared<Material> mat : m_materials)
	{
		if(mat->GetName() == _name)
			return mat;
	}

	if(!material)
		return LoadMaterial(_name);
}

weak<Material> Resources::LoadMaterialFromFile(std::string _name)
{
	shared<Material> material = std::make_shared<Material>(_name);

	m_materials.push_back(material);
	return material;
}

weak<Mesh> Resources::GetMesh(std::string _name)
{
	for(shared<Mesh> mesh : m_meshes)
	{
		if(mesh->GetName() == _name)
			return mesh;
	}
}

StoredModel Resources::LoadModel(std::string _model)
{
	StoredModel model;
	
	for(Uint i = 0; i < m_models.size(); i++)
	{
		if(m_models[i].GetName() == _model)
			return m_models.at(i);
	}
	
	model = LoadOBJFromFile(_model);

	return model;
}

void Resources::ReloadShader(std::string _name)
{
	weak<Shader> shader = LoadShader(_name);
	shader.lock()->ReloadShader();
	
	for(Uint i = 0; i < m_materials.size(); i++)
	{
		m_materials.at(i)->LinkToShader();
	}
}

StoredModel Resources::LoadOBJFromFile(std::string _name)
{
	std::string filepath = "Assets/Models/";

	//Sets object data to zero before loading
	int numVerts = 0;
	int numUVs = 0;
	int numNorms = 0;
	int numFaces = 0;
	
	std::ifstream inObjFile;
	std::string inObjData;	

	//Create temporary vectors to store model data as it's being loaded
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	glm::vec2 tempVec2;
	glm::vec3 tempVec3;

	std::vector<VertexIndex> object;
	VertexIndex bigIndex;

	StoredModel model(_name, _name.substr(0, _name.length() - 4));

	ModelPart modelPart;
	shared<Mesh> mesh;
	
	bool hasMat = false;

	std::string tempString;

	//Opens file parameter as input only and gives an error if it can't open it
	inObjFile.open((filepath + _name).c_str(), std::ifstream::in);

	if(inObjFile.is_open() != 1)
		printf("Could not load object file");

	modelPart.name = _name;

	//Reads from the file until the EOF is reached
	while(!inObjFile.eof())
	{
		//Get the next line from the file and convert it int oa stringstream
		std::getline(inObjFile, inObjData);
		std::stringstream objStream(inObjData);

		objStream >> inObjData;

		//The string is then parsed and data is extracted into the relevant vectors, dependant
		//upon what type of data is extracted
		if(inObjData == "")
		{
			continue;
		}

		else if(inObjData == "#")
		{
			objStream >> inObjData;

			if(inObjData == "object")
			{
				/*if(modelPart.name != "")
				{
					mesh = std::make_shared<Mesh>(vertices, normals, texCoords, object, modelPart.name);
					m_meshes.push_back(mesh);

					if(!hasMat)
					{
						modelPart.material = LoadMaterial("defaultMat");
					}

					hasMat = false;

					modelPart.mesh = mesh;
					model.AddModelPart(modelPart);
				}

				objStream >> inObjData;
				modelPart.name = inObjData;

				object.clear();*/
			}
		}

		else if(inObjData == "mtllib")
		{
			objStream >> inObjData;

			ParseMTL(inObjData);
		}

		else if(inObjData == "o")
		{
			/*if(modelPart.name != "")
			{
				mesh = std::make_shared<Mesh>(vertices, normals, texCoords, object, modelPart.name);
				m_meshes.push_back(mesh);

				if(!hasMat)
				{
					modelPart.material = LoadMaterial("defaultMat");
				}

				hasMat = false;

				modelPart.mesh = mesh;
				model.AddModelPart(modelPart);
			}

			objStream >> modelPart.name;

			object.clear();*/
		}

		else if(inObjData == "v")
		{
			objStream >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				
			vertices.push_back(tempVec3);
			numVerts++;
		}
				
		else if(inObjData == "vt")
		{
			objStream >> tempVec2.x >> tempVec2.y;

			texCoords.push_back(tempVec2);
			numUVs++;
		}

		else if(inObjData == "vn")
		{
			objStream >> tempVec3.x >> tempVec3.y >> tempVec3.z;
		
			normals.push_back(tempVec3);
			numNorms++;
		}

		else if (inObjData == "usemtl")
		{
			hasMat = true;

			objStream >> inObjData;
			modelPart.material = LoadMaterial(inObjData);
		}

		else if(inObjData == "g")
		{
			objStream >> modelPart.name;
		}

		else if(inObjData == "f")
		{
			std::streampos streamBegin = objStream.tellg();

			std::getline(objStream, inObjData);

			int numVertices = std::count(inObjData.begin(), inObjData.end(), '/') / 2;

			objStream.seekg(streamBegin);

			for(Uint i = 0; i < numVertices; i++)
			{
				if(i == 3)
				{
					object.push_back(object.at(object.size() - 3));
					object.push_back(object.at(object.size() - 2));

					numFaces++;
				}

				std::getline(objStream, inObjData, '/');	

				if(inObjData != "")
				{
					bigIndex.vertIndex = atoi(inObjData.c_str());
				}
				else
				{
					bigIndex.vertIndex = 0;
				}

				std::getline(objStream, inObjData, '/');

				if(inObjData != "")
				{
					bigIndex.texIndex = atoi(inObjData.c_str());
				}
				else
				{
					bigIndex.texIndex = 0;
				}

				std::getline(objStream, inObjData, ' ');

				if(inObjData != "")
				{
					bigIndex.normIndex = atoi(inObjData.c_str());
				}
				else
				{
					bigIndex.normIndex = 0;
				}

				numFaces++;
				object.push_back(bigIndex);
			}
		}
	}

	mesh = std::make_shared<Mesh>(vertices, normals, texCoords, object, modelPart.name);
	m_meshes.push_back(mesh);

	if(!hasMat)
	{
		modelPart.material = LoadMaterial("defaultMat");
	}

	modelPart.mesh = mesh;
	model.AddModelPart(modelPart);

	Vec3 center = GetCenterOfMesh(vertices);
	
	for(Uint i = 0; i < model.m_numModelParts; i++)
	{
		model.m_modelParts.at(i).offset = center;
		model.m_modelParts.at(i).mesh.lock()->AdjustVertices(center);
	}

	//Close the OBJ file as we are done with it
	inObjFile.close();

	m_models.push_back(model);

	//Display model data in the console window
	std::cout << "\n\nNumber of vertices: " << numVerts;
	std::cout << "\nNumber of faces: " << numFaces;
	std::cout << "\nNumber of UVs: " << numUVs;
	std::cout << "\nNumber of normals: " << numNorms << "\n";

	return model;
}

void Resources::ParseMTL(std::string _name)
{
	std::string filepath = "Assets/Models/";

	shared<Material> material;

	std::ifstream inMatFile;
	std::string inMatData;
	std::string texName;

	//Opens file parameter as input only and gives an error if it can't open it
	inMatFile.open((filepath + _name).c_str(), std::ifstream::in);

	if(inMatFile.is_open() != 1)
		printf("Could not load material file");

	//Reads from the file until the EOF is reached
	while(!inMatFile.eof())
	{
		//Get the next line from the file and convert it int oa stringstream
		std::getline(inMatFile, inMatData);
		std::stringstream matStream(inMatData);

		matStream >> inMatData;

		if(inMatData == "" || inMatData == "#")
			continue;

		else if(inMatData == "newmtl")
		{
			material = std::make_shared<Material>();
			material->m_shader = LoadShader("monolith");
			m_materials.push_back(material);

			matStream >> material->m_name;
		}

		else if(inMatData == "Ns")
		{
			matStream >> material->m_matProperties.specPower;
		}

		else if(inMatData == "Ka")
		{
			matStream >> material->m_matProperties.ambient.x >> material->m_matProperties.ambient.y >> material->m_matProperties.ambient.z;
		}

		else if(inMatData == "Kd")
		{
			matStream >> material->m_matProperties.diffuse.x >> material->m_matProperties.diffuse.y >> material->m_matProperties.diffuse.z;
		}

		else if(inMatData == "Ks")
		{
			matStream >> material->m_matProperties.specular.x >> material->m_matProperties.specular.y >> material->m_matProperties.specular.z;
		}

		else if(inMatData == "Ni")
		{
			matStream >> material->m_matProperties.refraction;
		}

		else if(inMatData == "d")
		{
			matStream >> material->m_matProperties.alpha;
		}

		else if(inMatData == "illum")
		{
			matStream >> material->m_matProperties.illumMode;
		}

		else if(inMatData == "map_Ka")
		{
			/*matStream >> texName;
			texName = filepath + texName;
			material->m_ambientTex = al_load_bitmap(texName.c_str());
			material->m_ambientTexGL = al_get_opengl_texture(material->m_ambientTex);*/
		}

		else if(inMatData == "map_Kd")
		{
			matStream >> texName;
			texName = filepath + texName;
			material->m_diffuseTex = al_load_bitmap(texName.c_str());

			material->m_hasTexture = true;
		}

		else if(inMatData == "map_Ks")
		{
			matStream >> texName;
			texName = filepath + texName;
			material->m_specularTex = al_load_bitmap(texName.c_str());
		}

		else if(inMatData == "map_Ns")
		{
			/*matStream >> texName;
			texName = filepath + texName;
			material->m_specularHighlight = al_load_bitmap(texName.c_str());
			material->m_specularHighlightGL = al_get_opengl_texture(material->m_specularHighlight);*/
		}

		else if(inMatData == "map_d")
		{
			/*matStream >> texName;
			texName = filepath + texName;
			material->m_displacementTex = al_load_bitmap(texName.c_str());
			material->m_displaceTexGL = al_get_opengl_texture(material->m_bumpTex);*/
		}

		else if(inMatData == "map_bump")
		{
			/*matStream >> texName;
			texName = filepath + texName;
			material->m_bumpTex = al_load_bitmap(texName.c_str());
			material->m_bumpTexGL = al_get_opengl_texture(material->m_bumpTex);*/
		}
	}

	inMatFile.close();
}

Vec3 Resources::GetCenterOfMesh(std::vector<Vec3>& _verts)
{
	float minX = _verts.at(0).x;
	float maxX = _verts.at(0).x;
	float minY = _verts.at(0).y;
	float maxY = _verts.at(0).y;
	float minZ = _verts.at(0).z;
	float maxZ = _verts.at(0).z;

	for(Uint i = 0; i < _verts.size(); i++)
	{
		if(_verts.at(i).x < minX)
			minX = _verts.at(i).x;

		if(_verts.at(i).x > maxX)
			maxX = _verts.at(i).x;

		if(_verts.at(i).y < minY)
			minY = _verts.at(i).y;

		if(_verts.at(i).y > maxY)
			maxY = _verts.at(i).y;

		if(_verts.at(i).z < minZ)
			minZ = _verts.at(i).z;

		if(_verts.at(i).z > maxZ)
			maxZ = _verts.at(i).z;
	}

	float midX = (maxX + minX) /2.0f;
	float midY = (maxY + minY) /2.0f;
	float midZ = (maxZ + minZ) /2.0f;

	return Vec3(midX, midY, midZ);
}