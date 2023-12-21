#pragma once

#include <rapidjson/document.h>

class SparseIndices
{
public:
    int bufferView;
    int byteOffset = 0;
    int componentType;

    SparseIndices() = default;

    explicit SparseIndices(const rapidjson::Value& value);
};
