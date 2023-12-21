#pragma once

#include <rapidjson/document.h>

class OcclusionTextureInfo
{
public:
    int index;
    int texCoord = 0;
    float strength = 1.f;

    OcclusionTextureInfo() = default;

    explicit OcclusionTextureInfo(const rapidjson::Value& value);
};
