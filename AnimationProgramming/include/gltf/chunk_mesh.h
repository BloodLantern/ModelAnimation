#pragma once

#include <string>
#include <vector>

#include "mesh_primitive.h"

#include "rapidjson/document.h"

class ChunkMesh
{
public:
    std::vector<MeshPrimitive> primitives;
    std::vector<float> weights;
    std::string name;

    ChunkMesh() = default;

    explicit ChunkMesh(const rapidjson::Value& value);
};
