#include "stdafx.h"

#include "Shader.h"

Shader::Shader()
{
	m_shaderFilePath = "Assets/Shaders/";
}

Shader::Shader(std::string _name)
{
	m_shaderFilePath = "Assets/Shaders/";

	m_fileName = _name;
	std::string vertShaderPath = "Vert.vert";
	std::string fragShaderPath = "Frag.frag";
	LoadShaders(m_fileName + vertShaderPath, m_fileName + fragShaderPath);
}

Shader::~Shader()
{

}

void Shader::LoadShaders(std::string _vertShaderPath, std::string _fragShaderPath)
{
	_vertShaderPath = m_shaderFilePath + _vertShaderPath;
	_fragShaderPath = m_shaderFilePath + _fragShaderPath;

	GLint program = glCreateProgram();

	GLint vertShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSource = LoadFileToString(_vertShaderPath);
	const char* rawVertShaderSource = vertShaderSource.c_str();
	glShaderSource(vertShader, 1, &rawVertShaderSource, NULL);
	glCompileShader(vertShader);
	glAttachShader(program, vertShader);

	GLint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSource = LoadFileToString(_fragShaderPath);
	const char* rawFragShaderSource = fragShaderSource.c_str();
	glShaderSource(fragShader, 1, &rawFragShaderSource, NULL);
	glCompileShader(fragShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	///Check program compiled
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	///If shaders fail to compile output the error message to console
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		std::cerr << _vertShaderPath << log << std::endl;
		delete [] log;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	m_shaderProgram =  program;
}

///This loads a file entered as an argument and returns the data from the
///file as one long string
std::string Shader::LoadFileToString(std::string _filePath)
{	
	std::string fileData;
	std::ifstream stream(_filePath, std::ios::in);

	///If the file opens then it gets lines from the file until it's no longer able to
	if(stream.is_open())
	{
		std::string line = "";

		while (getline(stream, line))
		{
			fileData += "\n" + line;
		}
	}

	///Close the file after using it
	stream.close();

	///Return the string of file data
	return fileData;
}

void Shader::ReloadShader()
{
	glDeleteProgram(m_shaderProgram);
	LoadShaders(m_fileName + "Vert.vert", m_fileName + "Frag.frag");
}

GLint Shader::GetShaderProgram()
{
	return m_shaderProgram;
}

std::string Shader::GetName()
{
	return m_fileName;
}

void Shader::ActivateShader()
{
	glUseProgram(m_shaderProgram);
}