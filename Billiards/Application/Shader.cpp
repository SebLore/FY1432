#include "Shader.h"

#include <fstream>
#include <iostream>


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const ShaderConfig &config) 
    : m_config(config)
{
    std::string vertexByteCode = loadSource(std::string(vertexPath));
    std::string fragmentByteCode = loadSource(std::string(fragmentPath));
    std::string geometryCode;
    if(geometryPath)
	{
		geometryCode = loadSource(std::string(geometryPath));
	}

	GLuint vertexID = compileShader(vertexByteCode, GL_VERTEX_SHADER);
	GLuint fragmentID = compileShader(fragmentByteCode, GL_FRAGMENT_SHADER);
	GLuint geometryID = 0;

	if (geometryPath)
		geometryID = compileShader(geometryCode, GL_GEOMETRY_SHADER);

	bool success = linkProgram(vertexID, fragmentID, geometryID);

	if (!success)
	{
		std::cerr << "Shader program linking failed. Exiting...\n";
		throw std::runtime_error("Shader program linking failed.");
	}

	std::cout << "Shader ready to use.\n";
}

Shader::~Shader() 
{
    if (m_ID != 0) {
        glDeleteProgram(m_ID);
    }
}

void Shader::Use() const 
{
    glUseProgram(m_ID);
}

// attempt to open and load the contents of a file to a string
std::string Shader::loadSource(const std::string& path) const
{
    std::string contents;
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to shader source from path " << path << ". Exiting...\n";
        return "";
    }

	ifs.seekg(0, std::ios::end); // go to end of file
	contents.reserve(static_cast<unsigned int>(ifs.tellg())); // reserve space for the string
	ifs.seekg(0, std::ios::beg); // go back to beginning of file

	contents.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()); // read the file into the string
	ifs.close(); // close the file

    // check if the contents are empty
	if (contents.empty())
	{
		std::cerr << "Shader source is empty. Exiting...\n";
		return "";
	}

	return contents;
}

GLuint Shader::compileShader(const std::string& source, ::GLenum type) const
{
	GLuint shader = glCreateShader(type);
	const char* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, NULL);
	glCompileShader(shader);
	checkCompileErrors(shader, 
		type == GL_VERTEX_SHADER ? "VERTEX" : 
		type == GL_FRAGMENT_SHADER ? "FRAGMENT" : 
		"GEOMETRY");
	return shader;
}

bool Shader::linkProgram(GLuint vertID, GLuint fragID, GLuint geoID)
{
	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertID);
	glAttachShader(m_ID, fragID);
	if (geoID != 0)
		glAttachShader(m_ID, geoID);

	glLinkProgram(m_ID);

	glDeleteShader(vertID);
	glDeleteShader(fragID);
	if (geoID != 0)
		glDeleteShader(geoID);

	// Check for linking errors
    return checkCompileErrors(m_ID, "PROGRAM");
}

bool Shader::checkCompileErrors(GLuint object, const std::string &type) const
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") 
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cerr << "ERROR::object_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }

    return true;
}