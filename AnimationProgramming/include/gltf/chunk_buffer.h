#pragma once

#include <string>

#include <rapidjson/document.h>

class ChunkBuffer
{
public:
    std::string uri;
    int byteLength;
    std::string name;

    ChunkBuffer() = default;

    explicit ChunkBuffer(const rapidjson::Value& value);
};
