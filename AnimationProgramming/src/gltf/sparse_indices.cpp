#include "gltf/sparse_indices.h"

#include "utils.h"

SparseIndices::SparseIndices(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(bufferView), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(byteOffset), value);
    
    utils::SetFromJson(VAR_AND_NAME(componentType), value);
}
