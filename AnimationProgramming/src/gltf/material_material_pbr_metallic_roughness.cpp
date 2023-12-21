#include "gltf/material_material_pbr_metallic_roughness.h"

#include "utils.h"

MaterialPbrMetallicRoughness::MaterialPbrMetallicRoughness(const rapidjson::Value& value)
{
    utils::SetFromJsonArraySafe(VAR_AND_NAME(baseColorFactor), value);
    
    utils::SetFromJsonClassSafe(VAR_AND_NAME(baseColorTexture), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(metallicFactor), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(roughnessFactor), value);
    
    utils::SetFromJsonClassSafe(VAR_AND_NAME(metallicRoughnessTexture), value);
}
