#include "gltf/chunk.h"

#include <iostream>
#include <rapidjson/document.h>

#include "utils.h"

bool Chunk::Load(std::ifstream& file)
{
    utils::Read(file, length);
    file.read(type, sizeof(type));

    if (std::strncmp(type, "JSON", sizeof(type)) == 0)
    {
        LoadJson(file);
    }
    else if (std::strncmp(type, "BIN ", sizeof(type)) == 0)
    {
        LoadBin(file);
    }
    else
    {
        std::cout << "Invalid glTF chunk type: " << type << ", expected 'JSON' or 'BIN '. Skipping\n";
        file.seekg(length, std::ios_base::cur);
        return false;
    }

    return true;
}

void Chunk::LoadJson(std::ifstream& file)
{
    std::string data;
    data.resize(length);
    file.read(data.data(), length);

    using namespace rapidjson;
    
    Document json;
    json.Parse(data.c_str());

    auto&& array = json["accessors"].GetArray();
    accessors.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
    {
        ChunkAccessor accessor;
        accessor.Load(array[i]);
        accessors[i] = accessor;
    }

    array = json["animations"].GetArray();
    animations.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
    {
        ChunkAnimation animation;
        animation.Load(array[i]);
        animations[i] = animation;
    }
}

void Chunk::LoadBin(std::ifstream& file)
{
}
