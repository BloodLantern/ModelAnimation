#include "gltf/accessor.h"

#include <string>

void Accessor::Load(const rapidjson::Value& object)
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

    auto&& array = object["max"].GetArray();
    max.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
        max[i] = array[i].GetFloat();

    array = object["min"].GetArray();
    min.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
        min[i] = array[i].GetFloat();
    
    name = object["name"].GetString();
}
