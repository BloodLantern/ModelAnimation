#include "gltf/chunk_accessor.h"

#include <string>

#include "utils.h"

ChunkAccessor::ChunkAccessor(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(bufferView), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(byteOffset), value);
    
    utils::SetFromJson(VAR_AND_NAME(componentType), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(normalized), value);
    
    utils::SetFromJson(VAR_AND_NAME(count), value);
    
    utils::SetFromJson(VAR_AND_NAME(type), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(max), value);
    
    utils::SetFromJsonArraySafe(VAR_AND_NAME(min), value);

    utils::SetFromJsonClassSafe(VAR_AND_NAME(sparse), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
