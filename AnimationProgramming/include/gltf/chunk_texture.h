#pragma once
#include <string>

#include "sparse_values.h"

class ChunkTexture
{
public:
    int sampler;
    int source;
    std::string name;

    ChunkTexture() = default;

    explicit ChunkTexture(const rapidjson::Value& value);
};
