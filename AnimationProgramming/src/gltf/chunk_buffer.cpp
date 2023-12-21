#include "gltf/chunk_buffer.h"

#include "utils.h"

ChunkBuffer::ChunkBuffer(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(uri), value);
    
    utils::SetFromJson(VAR_AND_NAME(byteLength), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
