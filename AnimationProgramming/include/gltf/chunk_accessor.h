#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>

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
    //accessor.sparse sparse; Not needed here
    std::string name;
    //extension extensions; Not needed here
    //extra extras; Not needed here

    void Load(const rapidjson::Value& object);
};
