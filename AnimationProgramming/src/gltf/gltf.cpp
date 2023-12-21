#include "gltf/gltf.h"

#include <iostream>
#include <rapidjson/document.h>

#include "utils.h"

bool Gltf::Load(std::ifstream& file)
{
    LoadHeader(file);
    
    while (!file.eof())
    {
        Chunk chunk;
        chunk.Load(file);
        m_Chunks.push_back(chunk);
    }

    return true;
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
