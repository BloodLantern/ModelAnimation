#include "mesh.h"

#include <fstream>
#include <iostream>

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
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * VertexFormatSize, m_Vertices.data(), GL_STATIC_DRAW);

    // Setup the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    // Generate VAO
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, VertexFormatSize, nullptr);
    glEnableVertexAttribArray(0);

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
}

bool Mesh::IsLoaded() const
{
    return m_Loaded;
}

void Mesh::LoadGltfFormat(std::ifstream& file)
{
}

void Mesh::LoadEngineFormat(std::ifstream& file)
{
    file.seekg(0);
    
    std::cout << "stream pos: " << file.tellg() << ":" << file.peek() << '\n';

    int vertexCount;
    file.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));
    
    std::cout << "stream pos: " << file.tellg() << ":" << file.peek() << '\n';

    std::cout << "Vertices: " << vertexCount << '\n';

    m_Vertices.resize(vertexCount);

    const unsigned int verticesSize = vertexCount * VertexFormatSize;
    char* vertices = static_cast<char*>(_malloca(verticesSize));
    file.read(vertices, verticesSize);
    std::memcpy(m_Vertices.data(), vertices, verticesSize);
    _freea(vertices);

    int partCount;
    file.read(reinterpret_cast<char*>(&partCount), sizeof(partCount));

    for (int i = 0; i < partCount; i++)
    {
        int indexCount;
        file.read(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));

        const size_t oldIndicesSize = m_Indices.size();
        m_Indices.reserve(indexCount);
        
        const unsigned int indicesSize = indexCount * sizeof(unsigned int);
        char* indices = static_cast<char*>(_malloca(indicesSize));
        file.read(indices, indicesSize);
        std::memcpy(reinterpret_cast<char*>(m_Indices.data()) + oldIndicesSize, indices, indicesSize);
        _freea(indices);

        // Here we should read the material but this mesh doesn't have one
    }
}
