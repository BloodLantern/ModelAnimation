#pragma once

#include <rapidjson/document.h>

class TextureInfo
{
public:
    int index;
    int texCoord = 0;

    TextureInfo() = default;

    explicit TextureInfo(const rapidjson::Value& value);
};
