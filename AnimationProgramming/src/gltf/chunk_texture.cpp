#include "gltf/chunk_texture.h"

#include "utils.h"

ChunkTexture::ChunkTexture(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(sampler), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(source), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
