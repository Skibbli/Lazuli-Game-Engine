#ifndef MATERIAL_H
#define MATERIAL_H

#include "stdafx.h"

#include <assimp/scene.h>	

#include "Shader.h"

/// Properties of the material
struct MaterialProperties
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 emissive;
	glm::vec3 specular;

	float specPower;
	float refraction;
	float alpha;

	int illumMode;
};

class Material
{
	friend class Resources;

	public:
		Material();
		Material(std::string _name);
		Material(aiMaterial* _mat, std::string _file);
		~Material();

		// Returns the name of the material
		std::string GetName();

		GLint GetShaderProgram();

		// Binds the texture for rendering
		void BindTexture();

		/// Returns whether the material has a texture or not
		bool HasTexture();

		/// Sets up the material and links to the shader
		void InitMaterial();
		/// Links the material to the shader
		void LinkToShader();

		/// makes the texture active for shading
		void ActivateMaterial();
		/// Sets the shader linked to this material
		void SetShader(weak<Shader> _shader);

	private:
		/// Resets the material
		void ResetMat();

		/// Whether or not the material has a texture
		bool m_hasTexture;
		/// The location of the texture on the GPU
		GLint m_hasTexLoc;

		GLint m_ambientColourLoc;
		GLint m_diffuseColourLoc;
		GLint m_emissiveColourLoc;
		GLint m_specularColourLoc;
		GLint m_specPowerLoc;
		GLint m_alphaLoc;
		GLint m_refractionLoc;

		//An int to store the location of the tex on the shader
		//GLuint m_ambientTexLoc;
		GLint m_diffuseTexLoc;
		GLint m_specularTexLoc;
		/*GLuint m_specularHighlightLoc;
		GLuint m_alphaTexLoc;
		GLuint m_bumpTexLoc;
		GLuint m_displaceTexLoc;*/

		//An int to store the texture of the object
		//GLuint m_ambientTexGL;
		GLint m_diffuseTexGL;
		GLint m_specularTexGL;
		/*GLuint m_specularHighlightGL;
		GLuint m_alphaTexGL;
		GLuint m_bumpTexGL;
		GLuint m_displaceTexGL;*/

		//ALLEGRO_BITMAP *m_ambientTex;
		ALLEGRO_BITMAP *m_diffuseTex;
		ALLEGRO_BITMAP *m_specularTex;
		/*ALLEGRO_BITMAP *m_specularHighlight;
		ALLEGRO_BITMAP *m_alphaTex;
		ALLEGRO_BITMAP *m_bumpTex;
		ALLEGRO_BITMAP *m_displacementTex;
		ALLEGRO_BITMAP *m_stencilTex;*/

		//Name of this material
		std::string m_name;

		/// Properties of the material
		MaterialProperties m_matProperties;

		/// The shader linked to this material
		weak<Shader> m_shader;
};

#endif