#include "mesh.h"

#include <fstream>
#include <iostream>
#include <filesystem>

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
        LoadGltfFormat(file, filename);
    else
        std::cout << "Invalid file format: " << std::string(magic, 4) << '\n';

    m_Loaded = true;
}

void Mesh::Unload()
{
    if (!m_Loaded)
        return;

    glDeleteBuffers(2, &m_Vbo);
    glDeleteVertexArrays(1, &m_Vao);

    glDeleteTextures(static_cast<GLsizei>(m_TextureIds.size()), m_TextureIds.data());

    delete m_Gltf;
    
    m_Vertices.clear();
    m_Indices.clear();

    m_Loaded = false;
}

void Mesh::Forward()
{
    if (!m_Loaded)
        return;
    
    glGenBuffers(2, &m_Vbo);
    
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);

    if (m_Gltf)
    {
        const ChunkBin* bin = m_Gltf->GetBinaryChunk();

        const std::vector<ChunkBin::MeshData>& meshDatas = bin->GetMeshes();

        if (meshDatas.size() > 1)
            std::cout << "Currently cannot load more than one mesh in a single glTF file\n";

        const ChunkBin::MeshData& meshData = meshDatas[0];

        const std::vector<ChunkBin::MeshPrimitiveData>& meshPrimitiveDatas = meshData.primitives;

        if (meshPrimitiveDatas.size() > 1)
            std::cout << "Currently cannot load more than one primitive in a single glTF mesh\n";

        const ChunkBin::MeshPrimitiveData& meshPrimitiveData = meshPrimitiveDatas[0];

        const std::vector<ChunkBin::BufferView>& bufferViews = bin->GetBufferViews();
        
        {
            {
                const ChunkBin::BufferView& positions = bufferViews[meshPrimitiveData.attributes.at("POSITION")];
                
                if (positions.bufferView->target != GL_ARRAY_BUFFER)
                    std::cout << "Invalid target for POSITION attribute: " << positions.bufferView->target << ", expected: " << GL_ARRAY_BUFFER << '\n';

                const ChunkBin::BufferView& texCoord = bufferViews[meshPrimitiveData.attributes.at("TEXCOORD_0")];
                
                m_Vertices.resize(positions.length / sizeof(vec3));
                for (unsigned int i = 0; i < m_Vertices.size(); i++)
                {
                    m_Vertices[i] = Vertex
                    {
                        .position = *reinterpret_cast<const vec3* const>(positions.ptr + i * sizeof(vec3)),
                        .texCoord = *reinterpret_cast<const vec2* const>(texCoord.ptr + i * sizeof(vec2))
                    };
                }
            }

            {
                const ChunkBin::BufferView& indices = bufferViews[meshPrimitiveData.indices];
            
                if (indices.bufferView->target == GL_ELEMENT_ARRAY_BUFFER)
                {
                    m_Indices.resize(indices.length / sizeof(unsigned short));
                    std::memcpy(m_Indices.data(), indices.ptr, indices.length);
                }
            }

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Vertices.size()) * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Indices.size()) * sizeof(unsigned short), m_Indices.data(), GL_STATIC_DRAW);
        }

        {
            const std::vector<ChunkBin::AccessorBuffer>& accessorBuffers = bin->GetAccessorBuffers();
            
            for (const auto& accessor : accessorBuffers)
            {
                if (accessor.accessor->name == "POSITION")
                {
                    glVertexAttribPointer(0, static_cast<GLsizei>(accessor.componentCount), accessor.componentType, accessor.normalized, sizeof(Vertex), nullptr);
                    glEnableVertexAttribArray(0);
                }
                else if (accessor.accessor->name == "TEXCOORD_0")
                {
                    glVertexAttribPointer(1, static_cast<GLsizei>(accessor.componentCount), accessor.componentType, accessor.normalized, sizeof(Vertex), reinterpret_cast<void*>(sizeof(vec3)));
                    glEnableVertexAttribArray(1);
                }
            }
        }

        {
            const std::vector<ChunkBin::ImageData>& images = bin->GetImages();
            
            m_TextureIds.resize(images.size());
            glGenTextures(static_cast<GLsizei>(images.size()), m_TextureIds.data());
            
            for (unsigned int i = 0; i < images.size(); i++)
            {
                const ChunkBin::ImageData& image = images[i];
                const GLuint& textureId = m_TextureIds[i];
                
                glBindTexture(GL_TEXTURE_2D, textureId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
            }
            
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

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

    glBindTexture(GL_TEXTURE_2D, m_TextureIds[0]);
    glBindVertexArray(m_Vao);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Mesh::IsLoaded() const
{
    return m_Loaded;
}

void Mesh::LoadGltfFormat(std::ifstream& file, const std::filesystem::path& filepath)
{
    m_Gltf = new Gltf(file, filepath);
}
