#pragma once

#include <vector>

#include <rapidjson/document.h>

class MeshPrimitive
{
public:
    rapidjson::Document attributes;
    int indices;
    int material;
    int mode = 4;
    std::vector<rapidjson::Document> targets;

    MeshPrimitive() = default;

    explicit MeshPrimitive(const rapidjson::Value& value);
};
