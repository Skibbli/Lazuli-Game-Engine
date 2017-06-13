#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
	friend class Material;

	public:
		Shader();
		Shader(std::string _name);
		~Shader();

		/// Loads the shaders from the given filepaths
		void LoadShaders(std::string _vertShaderPath, std::string _fragShaderPath);
		/// Loads the text from the given file in to one big string
		std::string LoadFileToString(std::string _filePath);

		/// Reloads the shader while the engine is running, speeding up shader development
		void ReloadShader();

		//// Returns the ID of the shader program
		GLint GetShaderProgram();
		/// Returns the name of the shader
		std::string GetName();

		/// Activates the shader
		void ActivateShader();

	private:
		/// The filepath of the shader
		std::string m_shaderFilePath;

		///Name of this shader
		std::string m_fileName;

		/// The location of the shader program on the GPU
		GLint m_shaderProgram;
};

#endif