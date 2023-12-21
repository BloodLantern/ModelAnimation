#include "gltf/sampler.h"

void Sampler::Load(const rapidjson::Value& value)
{
    input = value["input"].GetInt();

    if (value.HasMember("interpolation"))
        interpolation = value["interpolation"].GetString();

    output = value["output"].GetInt();
}
