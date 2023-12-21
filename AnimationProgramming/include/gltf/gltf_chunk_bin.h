#pragma once

#include "gltf_chunk.h"

class ChunkBin : public Chunk
{
public:
    ChunkBin() = default;
    
    explicit ChunkBin(unsigned int length, char type[4], std::ifstream& file);
};
