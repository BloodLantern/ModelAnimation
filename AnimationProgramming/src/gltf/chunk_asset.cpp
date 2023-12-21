#include "gltf/chunk_asset.h"

#include "utils.h"

ChunkAsset::ChunkAsset(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(copyright), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(generator), value);
    
    utils::SetFromJson(VAR_AND_NAME(version), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(minVersion), value);
}
