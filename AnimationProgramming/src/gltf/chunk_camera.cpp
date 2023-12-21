#include "gltf/chunk_camera.h"

#include "utils.h"

ChunkCamera::ChunkCamera(const rapidjson::Value& value)
{
    utils::SetFromJsonClassSafe(VAR_AND_NAME(orthographic), value);
    
    utils::SetFromJsonClassSafe(VAR_AND_NAME(perspective), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(type), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
