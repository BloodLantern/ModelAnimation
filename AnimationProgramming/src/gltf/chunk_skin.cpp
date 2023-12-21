#include "gltf/chunk_skin.h"

#include "utils.h"

ChunkSkin::ChunkSkin(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(inverseBindMatrices), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(skeleton), value);
    
    utils::SetFromJsonArray(VAR_AND_NAME(joints), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
