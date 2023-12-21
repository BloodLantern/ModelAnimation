#include "gltf/material_normal_texture_info.h"

#include "utils.h"

NormalTextureInfo::NormalTextureInfo(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(index), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(texCoord), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(scale), value);
}
