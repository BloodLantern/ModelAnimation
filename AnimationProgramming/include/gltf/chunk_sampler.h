#pragma once

#include <string>

#include "rapidjson/document.h"

class ChunkSampler
{
public:
    int magFilter;
    int minFilter;
    int wrapS = 10497;
    int wrapT = 10497;
    std::string name;

    ChunkSampler() = default;

    explicit ChunkSampler(const rapidjson::Value& value);
};
