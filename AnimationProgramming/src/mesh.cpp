#include "mesh.h"

#include <fstream>
#include <iostream>

#include "utils.h"
#include "gltf/gltf.h"
#include "maths/vector4.hpp"

// We could replicate the behavior of the engine's code but because there is
// only one mesh being loaded in this format it is easier to hardcode this
constexpr int VertexFormatSize = 56;

Mesh::Mesh(const std::string& filename)
{
    Load(filename);
}

Mesh::~Mesh()
{
    if (m_Loaded)
        Unload();
}

void Mesh::Load(const std::string& filename)
{
    if (m_Loaded)
        return;
    
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open() || !file.good())
    {
        std::cout << "Couldn't open file: " << filename << '\n';
        return;
    }

    char magic[4];
    file.read(magic, sizeof(magic));

    if (std::strncmp(magic, "glTF", sizeof(magic)) == 0)
        LoadGltfFormat(file);
    else
        LoadEngineFormat(file);

    m_Loaded = true;
}

void Mesh::Unload()
{
    if (!m_Loaded)
        return;
    
    m_Vertices.clear();
    m_Indices.clear();

    glDeleteBuffers(2, &m_Vbo);
    glDeleteVertexArrays(1, &m_Vao);

    m_Loaded = false;
}

void Mesh::Forward()
{
    if (!m_Loaded)
        return;
    
    // Generate VBO and EBO
    glGenBuffers(2, &m_Vbo);
    
    // Setup the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Vertices.size()) * static_cast<GLsizeiptr>(sizeof(vec3)), m_Vertices.data(), GL_STATIC_DRAW);

    // Setup the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Indices.size()) * static_cast<GLsizeiptr>(sizeof(unsigned int)), m_Indices.data(), GL_STATIC_DRAW);

    // Generate VAO
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(vec4), reinterpret_cast<void*>(sizeof(vec3) * 2));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(vec4), reinterpret_cast<void*>(sizeof(vec3) * 2 + sizeof(vec4)));
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);
    // And the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw() const
{
    if (!m_Loaded)
        return;
    
    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(m_Vertices.size()));

    glBindVertexArray(0);
}

bool Mesh::IsLoaded() const
{
    return m_Loaded;
}

void Mesh::LoadGltfFormat(std::ifstream& file)
{
    Gltf gltf(file);
}

void Mesh::LoadEngineFormat(std::ifstream& file)
{
    file.seekg(0);

    int vertexCount;
    utils::Read(file, vertexCount);

    // Skip the engine's flags
    file.seekg(sizeof(int) * 2ll, std::ios_base::cur);

    m_Vertices.resize(vertexCount);

    const unsigned int verticesSize = vertexCount * VertexFormatSize;
    char* vertices = static_cast<char*>(_malloca(verticesSize));
    file.read(vertices, verticesSize);
    for (int i = 0; i < vertexCount; i++)
    {
        m_Vertices[i] = *reinterpret_cast<vec3*>(vertices + i * VertexFormatSize);
        std::swap(m_Vertices[i].y, m_Vertices[i].z);
    }
    _freea(vertices);

    int partCount;
    utils::Read(file, partCount);

    for (int i = 0; i < partCount; i++)
    {
        int indexCount;
        utils::Read(file, indexCount);

        const size_t oldIndicesSize = m_Indices.size();
        m_Indices.resize(m_Indices.size() + indexCount);
        
        const unsigned int indicesSize = indexCount * sizeof(unsigned int);
        char* indices = static_cast<char*>(_malloca(indicesSize));
        file.read(indices, indicesSize);
        std::memcpy(reinterpret_cast<char*>(m_Indices.data()) + oldIndicesSize, indices, indicesSize);
        _freea(indices);

        // Here we should read the material but this mesh doesn't have one so we just skip 4 bytes (material name length == 0)
        file.seekg(sizeof(int), std::ios_base::cur);
    }
}
