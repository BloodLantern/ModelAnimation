#include "gltf/sparse_values.h"

#include "utils.h"

SparseValues::SparseValues(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(bufferView), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(byteOffset), value);
}
