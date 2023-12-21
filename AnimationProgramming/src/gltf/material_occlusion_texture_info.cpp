#include "gltf/material_occlusion_texture_info.h"

#include "utils.h"

OcclusionTextureInfo::OcclusionTextureInfo(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(index), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(texCoord), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(strength), value);
}
