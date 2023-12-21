#include "gltf/material_texture_info.h"

#include "utils.h"

TextureInfo::TextureInfo(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(index), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(texCoord), value);
}
