#pragma once

#include <string>

#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

struct Texture
{
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
    Mesh() = default;
    Mesh(const Vertex* vertices, const GLsizei* indices, const GLsizei vertexCount, const GLsizei indexCount);
    ~Mesh()
    {
        Cleanup();
    }

    // copy and move
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& mesh)noexcept;
    Mesh& operator=(Mesh&& mesh)noexcept;

    void Init(const Vertex* vertices, const GLsizei* indices, const GLsizei vertexCount, const GLsizei indexCount);
    void Draw()const;
private:
    void Cleanup()
    {
        if (m_VAO != 0)
            glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO != 0)
            glDeleteBuffers(1, &m_VBO);
        if (m_EBO != 0)
            glDeleteBuffers(1, &m_EBO);
        m_VAO = m_VBO = m_EBO = 0;
    }
    
    GLuint m_VAO = 0; // vertex array object https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object 
    GLuint m_VBO = 0; // vertex buffer object https://en.wikipedia.org/wiki/Vertex_buffer_object
    GLuint m_EBO = 0; // EBO index buffer 
    GLsizei m_vertexCount = 0;
    GLsizei m_indexCount = 0;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

};
