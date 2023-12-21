#pragma once

#include <string>
#include <vector>

#include "rapidjson/document.h"

class ChunkSkin
{
public:
    int inverseBindMatrices;
    int skeleton;
    std::vector<int> joints;
    std::string name;

    ChunkSkin() = default;

    explicit ChunkSkin(const rapidjson::Value& value);
};
