#include "gltf/chunk_node.h"

#include "utils.h"

ChunkNode::ChunkNode(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(camera), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(children), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(skin), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(matrix), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(mesh), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(rotation), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(scale), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(weights), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
