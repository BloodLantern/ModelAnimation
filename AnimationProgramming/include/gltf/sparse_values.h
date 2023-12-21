#pragma once

#include <rapidjson/document.h>

class SparseValues
{
public:
    int bufferView;
    int byteOffset = 0;
    
    SparseValues() = default;
    
    explicit SparseValues(const rapidjson::Value& value);
};
