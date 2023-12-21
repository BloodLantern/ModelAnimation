#pragma once

#include <vector>

#include <rapidjson/document.h>

#include "material_texture_info.h"

class MaterialPbrMetallicRoughness
{
public:
    std::vector<float> baseColorFactor = { 1.f, 1.f, 1.f, 1.f };
    TextureInfo baseColorTexture;
    float metallicFactor = 1.f;
    float roughnessFactor = 1.f;
    TextureInfo metallicRoughnessTexture;

    MaterialPbrMetallicRoughness() = default;

    explicit MaterialPbrMetallicRoughness(const rapidjson::Value& value);
};
