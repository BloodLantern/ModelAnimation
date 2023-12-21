#include "gltf/animation_channel.h"

#include "utils.h"

Channel::Channel(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(sampler), value);

    utils::SetFromJsonClass(VAR_AND_NAME(target), value);
}
