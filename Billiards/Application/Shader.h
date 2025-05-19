#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp> // For glm types used in uniform setters

struct ShaderConfig
{
    std::unordered_map<std::string, std::string> defines; // <shader, define> TODO: implement this in compileShader to pass custom defines to the shader.
    bool verboseCompileErrors = true;
};

class Shader
{
public:

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const ShaderConfig &config = ShaderConfig());
    ~Shader();

    // Prevent copying/moving
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept : m_ID(other.m_ID) { other.m_ID = 0; } // Allow move
    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            glDeleteProgram(m_ID);
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        return *this;
    }

    // Use/activate the shader
    void Use() const;

    unsigned int GetID()const { return m_ID; }

    void SetBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value); }
    void SetInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value); }
    void SetFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value); }
    void SetVec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]); }
    void SetMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

private:
    ::GLuint m_ID = 0;
    ShaderConfig m_config;

    std::string loadSource(const std::string& path)const;
	GLuint compileShader(const std::string& source, ::GLenum type)const;
    bool linkProgram(GLuint vertID, GLuint fragID, GLuint geoID);
    bool checkCompileErrors(GLuint object, const std::string& type)const;
};