#include "gltf/chunk_scene.h"

#include "utils.h"

ChunkScene::ChunkScene(const rapidjson::Value& value)
{
    utils::SetFromJsonArraySafe(VAR_AND_NAME(nodes), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
