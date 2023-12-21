#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>

#include "accessor_sparse.h"

class ChunkAccessor
{
public:
    int bufferView;
    int byteOffset = 0;
    int componentType;
    bool normalized = false;
    int count;
    std::string type;
    // Size between 1 and 16
    std::vector<float> max;
    // Size between 1 and 16
    std::vector<float> min;
    AccessorSparse sparse;
    std::string name;

    ChunkAccessor() = default;
    
    explicit ChunkAccessor(const rapidjson::Value& value);
};
