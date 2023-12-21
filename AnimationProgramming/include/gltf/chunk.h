#pragma once

#include <vector>

#include "accessor.h"
#include "gltf/animation.h"

class Chunk
{
public:
    unsigned int length;
    char type[4];

    std::vector<Accessor> accessors;
    std::vector<Animation> animations;
    
    bool Load(std::ifstream& file);

private:
    bool LoadJson(std::ifstream& file);
    bool LoadBin(std::ifstream& file);
};
