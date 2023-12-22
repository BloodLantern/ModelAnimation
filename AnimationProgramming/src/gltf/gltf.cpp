#include "gltf/gltf.h"

#include <iostream>

#include "utils.h"
#include "gltf/gltf_chunk_bin.h"
#include "gltf/gltf_chunk_json.h"

Gltf::Gltf(std::ifstream& file, const std::filesystem::path& filepath)
{
    LoadHeader(file);
    
    while (file.peek() != EOF)
    {
        unsigned int length;
        utils::Read(file, length);
        char type[4];
        utils::Read(file, type);

        Chunk* chunk = nullptr;
        if (std::strncmp(type, "JSON", sizeof(type)) == 0)
        {
            chunk = new ChunkJson(length, type, file);
        }
        else if (std::strncmp(type, "BIN\0", sizeof(type)) == 0)
        {
            chunk = new ChunkBin(length, type, file, reinterpret_cast<ChunkJson*>(m_Chunks[m_Chunks.size() - 1]), filepath);
        }
        else
        {
            std::cout << "Invalid glTF chunk type: " << type << ", expected 'JSON' or 'BIN '. Skipping\n";
            file.seekg(length, std::ios_base::cur);
        }

        if (chunk)
            m_Chunks.push_back(chunk);
    }
}

Gltf::~Gltf()
{
    for (const Chunk* const chunk : m_Chunks)
        delete chunk;
}

const ChunkJson* Gltf::GetJsonChunk(const int index) const
{
    return reinterpret_cast<ChunkJson*>(m_Chunks[index * 2]);
}

const ChunkBin* Gltf::GetBinaryChunk(const int index) const
{
    return reinterpret_cast<ChunkBin*>(m_Chunks[index * 2 + 1]);
}

bool Gltf::LoadHeader(std::ifstream& file)
{
    utils::Read(file, m_Version);

    if (m_Version != 2)
    {
        std::cout << "Invalid glTF version: " << m_Version << ", expected 2\n";
        return false;
    }

    utils::Read(file, m_FileSize);

    return true;
}
