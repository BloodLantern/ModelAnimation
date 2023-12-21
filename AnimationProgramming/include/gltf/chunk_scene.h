#pragma once

#include <string>
#include <vector>

#include "rapidjson/document.h"

class ChunkScene
{
public:
    std::vector<int> nodes;
    std::string name;

    ChunkScene() = default;

    explicit ChunkScene(const rapidjson::Value& value);
};
