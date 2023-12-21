#include "gltf/chunk_sampler.h"

#include "utils.h"

ChunkSampler::ChunkSampler(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(magFilter), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(minFilter), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(wrapS), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(wrapT), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
