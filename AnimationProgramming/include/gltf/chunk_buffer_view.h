#pragma once

#include <string>

#include <rapidjson/document.h>

class ChunkBufferView
{
public:
    int buffer;
    int byteOffset = 0;
    int byteLength;
    int byteStride;
    int target;
    std::string name;

    ChunkBufferView() = default;

    explicit ChunkBufferView(const rapidjson::Value& value);
};
