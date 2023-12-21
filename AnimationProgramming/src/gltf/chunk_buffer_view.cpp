#include "gltf/chunk_buffer_view.h"

#include "utils.h"

ChunkBufferView::ChunkBufferView(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(buffer), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(byteOffset), value);
    
    utils::SetFromJson(VAR_AND_NAME(byteLength), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(byteStride), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(target), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
