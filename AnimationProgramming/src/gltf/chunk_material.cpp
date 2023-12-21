#include "gltf/chunk_material.h"

#include "utils.h"

ChunkMaterial::ChunkMaterial(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(name), value);

    utils::SetFromJsonClassSafe(VAR_AND_NAME(pbrMetallicRoughness), value);

    utils::SetFromJsonClassSafe(VAR_AND_NAME(normalTexture), value);

    utils::SetFromJsonClassSafe(VAR_AND_NAME(occlusionTexture), value);

    utils::SetFromJsonClassSafe(VAR_AND_NAME(emissiveTexture), value);

    utils::SetFromJsonArraySafe(VAR_AND_NAME(emissiveFactor), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(alphaMode), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(alphaCutoff), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(doubleSided), value);
}
