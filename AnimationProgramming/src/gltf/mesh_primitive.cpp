#include "gltf/mesh_primitive.h"

#include "utils.h"

MeshPrimitive::MeshPrimitive(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(attributes), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(indices), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(material), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(mode), value);

    utils::SetFromJsonArraySafe(VAR_AND_NAME(targets), value);
}
