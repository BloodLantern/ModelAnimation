#pragma once

#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "material_material_pbr_metallic_roughness.h"
#include "material_normal_texture_info.h"
#include "material_occlusion_texture_info.h"
#include "material_texture_info.h"

class ChunkMaterial
{
public:
    std::string name;
    MaterialPbrMetallicRoughness pbrMetallicRoughness;
    NormalTextureInfo normalTexture;
    OcclusionTextureInfo occlusionTexture;
    TextureInfo emissiveTexture;
    std::vector<float> emissiveFactor = { 0.f, 0.f, 0.f };
    std::string alphaMode = "OPAQUE";
    float alphaCutoff = 0.5f;
    bool doubleSided = false;

    ChunkMaterial() = default;

    explicit ChunkMaterial(const rapidjson::Value& value);
};
