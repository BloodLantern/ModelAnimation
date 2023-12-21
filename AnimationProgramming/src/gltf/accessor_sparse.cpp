#include "gltf/accessor_sparse.h"

#include "utils.h"

AccessorSparse::AccessorSparse(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(count), value);

    utils::SetFromJsonClass(VAR_AND_NAME(indices), value);
    
    utils::SetFromJsonClass(VAR_AND_NAME(values), value);
}
