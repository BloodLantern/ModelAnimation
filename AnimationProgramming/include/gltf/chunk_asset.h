#pragma once

#include <string>

#include "rapidjson/document.h"

class ChunkAsset
{
public:
    std::string copyright;
    std::string generator;
    std::string version;
    std::string minVersion;

    ChunkAsset() = default;

    explicit ChunkAsset(const rapidjson::Value& value);
};
