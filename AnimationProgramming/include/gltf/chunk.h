#pragma once

#include <vector>

#include "chunk_accessor.h"
#include "chunk_animation.h"

class Chunk
{
public:
    unsigned int length;
    char type[4];

    std::vector<ChunkAccessor> accessors;
    std::vector<ChunkAnimation> animations;
    
    bool Load(std::ifstream& file);

private:
    void LoadJson(std::ifstream& file);
    void LoadBin(std::ifstream& file);
};
