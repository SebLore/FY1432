#pragma once

#include <string>
#include <glm/glm.hpp> // For glm types used in uniform setters

// Forward declare GLAD's types if not including glad.h here (though often simpler to include)
// If you don't include glad.h, you'd need: typedef unsigned int GLuint; etc.
// It's often okay to include glad.h in utility headers like this.
#include <glad/gl.h>


class Shader
{
public:
    // Program ID
    unsigned int ID = 0;

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    // Destructor
    ~Shader();

    // Prevent copying/moving
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept : ID(other.ID) { other.ID = 0; } // Allow move
    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            glDeleteProgram(ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }


    // Use/activate the shader
    void Use() const;

    // Utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(GLuint shader, std::string type);
};