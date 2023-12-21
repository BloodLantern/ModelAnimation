#pragma once

#include <string>
#include <vector>

#include <rapidjson/document.h>

class ChunkNode
{
public:
    int camera;
    std::vector<int> children;
    int skin;
    // Here we don't use the math type Matrix4x4 as it is easier to read from the JSON
    // using a std::vector and convert it to a matrix when getting the data in the application
    std::vector<float> matrix = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    int mesh;
    // Same here, we use a std::vector instead of a Quaternion
    std::vector<float> rotation = { 0, 0, 0, 1 };
    // Same instead of a Vector3
    std::vector<float> scale = { 1, 1, 1 };
    // Same
    std::vector<float> translation = { 0, 0, 0 };
    std::vector<float> weights;
    std::string name;

    ChunkNode() = default;

    explicit ChunkNode(const rapidjson::Value& value);
};
