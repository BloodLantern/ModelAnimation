#include "gltf/gltf_chunk_json.h"

#include "utils.h"

ChunkJson::ChunkJson(const unsigned int length, char type[4], std::ifstream& file)
    : Chunk(length, type)
{
    std::string data;
    utils::Read(file, data, length);
    
    rapidjson::Document json;
    json.Parse(data.c_str());

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(accessors), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(animations), json);

    utils::SetFromJsonClass(VAR_AND_NAME(asset), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(buffers), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(bufferViews), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(cameras), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(images), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(materials), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(meshes), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(nodes), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(samplers), json);

    utils::SetFromJsonSafe(VAR_AND_NAME(scene), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(scenes), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(skins), json);

    utils::SetFromJsonArrayClassSafe(VAR_AND_NAME(textures), json);
}
