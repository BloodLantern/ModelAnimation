#include "gltf/chunk_mesh.h"

#include "utils.h"

ChunkMesh::ChunkMesh(const rapidjson::Value& value)
{
    utils::SetFromJsonArrayClass(VAR_AND_NAME(primitives), value);

    utils::SetFromJsonArraySafe(VAR_AND_NAME(weights), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);
}
