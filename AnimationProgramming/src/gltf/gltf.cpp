#include "gltf/gltf.h"

#include <iostream>

#include "utils.h"
#include "gltf/gltf_chunk_bin.h"
#include "gltf/gltf_chunk_json.h"

Gltf::Gltf(std::ifstream& file)
{
    LoadHeader(file);
    
    while (file.tellg() < m_FileSize)
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
        else if (std::strncmp(type, "BIN ", sizeof(type)) == 0)
        {
            chunk = new ChunkBin(length, type, file);
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
