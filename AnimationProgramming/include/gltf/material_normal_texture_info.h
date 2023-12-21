#pragma once

#include <rapidjson/document.h>

class NormalTextureInfo
{
public:
    int index;
    int texCoord = 0;
    float scale = 1.f;

    NormalTextureInfo() = default;

    explicit NormalTextureInfo(const rapidjson::Value& value);
};
