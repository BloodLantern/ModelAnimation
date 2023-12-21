#pragma once

#include <string>

#include <rapidjson/document.h>

class ChunkImage
{
public:
    std::string uri;
    std::string mimeType;
    int bufferView;
    std::string name;

    ChunkImage() = default;

    explicit ChunkImage(const rapidjson::Value& value);
};
