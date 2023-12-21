#include "..\..\include\gltf\chunk_accessor.h"

#include <string>

void ChunkAccessor::Load(const rapidjson::Value& object)
{
    if (object.HasMember("bufferView"))
        bufferView = object["bufferView"].GetInt();
    
    if (object.HasMember("byteOffset"))
        byteOffset = object["byteOffset"].GetInt();
    
    componentType = object["componentType"].GetInt();
    
    if (object.HasMember("normalized"))
        normalized = object["normalized"].GetBool();
    
    count = object["count"].GetInt();
    
    type = object["type"].GetString();

    const auto& maxArray = object["max"].GetArray();
    max.resize(maxArray.Size());
    for (unsigned int i = 0; i < maxArray.Size(); i++)
        max[i] = maxArray[i].GetFloat();

    const auto& minArray = object["min"].GetArray();
    min.resize(minArray.Size());
    for (unsigned int i = 0; i < minArray.Size(); i++)
        min[i] = minArray[i].GetFloat();
    
    name = object["name"].GetString();
}
