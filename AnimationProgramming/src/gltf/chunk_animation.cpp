#include "gltf/chunk_animation.h"

#include "utils.h"

ChunkAnimation::ChunkAnimation(const rapidjson::Value& value)
{
    utils::SetFromJsonArrayClass(VAR_AND_NAME(channels), value);
    
    utils::SetFromJsonArrayClass(VAR_AND_NAME(samplers), value);
}
