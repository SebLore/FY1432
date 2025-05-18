#include "Mesh.h"

// mesh constructor
Mesh::Mesh(const Vertex* vertices, const GLsizei* indices, const GLsizei vertexCount, const GLsizei indexCount)
{
    Init(vertices, indices, vertexCount, indexCount);
}

Mesh::Mesh(Mesh&& mesh) noexcept
{
    m_VAO = mesh.m_VAO;
    m_VBO = mesh.m_VBO;
    m_EBO = mesh.m_EBO;
    m_vertexCount = mesh.m_vertexCount;
    m_indexCount = mesh.m_indexCount;

    mesh.m_VAO = mesh.m_VBO = mesh.m_EBO = 0;
    mesh.m_vertexCount = mesh.m_indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
    if (this != &mesh)
    {
        m_VAO = mesh.m_VAO;
        m_VBO = mesh.m_VBO;
        m_EBO = mesh.m_EBO;
        m_vertexCount = mesh.m_vertexCount;
        m_indexCount = mesh.m_indexCount;

        mesh.m_VAO = mesh.m_VBO = mesh.m_EBO = 0;
        mesh.m_vertexCount = mesh.m_indexCount = 0;
    }
    return *this;
}

void Mesh::Init(const Vertex* vertices, const GLsizei* indices, const GLsizei vertexCount, const GLsizei indexCount)
{
    m_vertexCount = vertexCount;
    m_indexCount = indexCount;

    // create VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // create vertex buffer
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // create index buffer
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLsizei), indices, GL_STATIC_DRAW);

    // vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0); // unbind VAO for now
}

// draw function assuming the shader is set outside the mesh
void Mesh::Draw() const
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

