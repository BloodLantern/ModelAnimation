#include "..\..\include\gltf\animation_sampler.h"

#include "utils.h"

Sampler::Sampler(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(input), value);

    utils::SetFromJsonSafe(VAR_AND_NAME(interpolation), value);

    utils::SetFromJson(VAR_AND_NAME(output), value);
}
