#include "gltf/chunk_image.h"

#include "utils.h"

ChunkImage::ChunkImage(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(uri), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(mimeType), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(bufferView), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
