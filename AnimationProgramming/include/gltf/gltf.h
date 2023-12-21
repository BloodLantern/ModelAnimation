#pragma once

#include <fstream>
#include <vector>

#include "chunk.h"

class Gltf
{
public:
    bool Load(std::ifstream& file);

private:
    unsigned int m_Version;
    unsigned int m_FileSize;
    
    std::vector<Chunk> m_Chunks;

    bool LoadHeader(std::ifstream& file);
};
