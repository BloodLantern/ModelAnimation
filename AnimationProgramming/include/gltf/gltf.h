#pragma once

#include <fstream>
#include <vector>

#include "gltf_chunk.h"

class Gltf
{
public:
    explicit Gltf(std::ifstream& file);

    ~Gltf();

private:
    unsigned int m_Version;
    unsigned int m_FileSize;
    
    std::vector<Chunk*> m_Chunks;

    bool LoadHeader(std::ifstream& file);
};
