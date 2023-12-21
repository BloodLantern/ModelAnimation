#include "gltf/channel.h"

void Channel::Load(const rapidjson::Value& value)
{
    sampler = value["sampler"].GetInt();

    target.Load(value["target"]);
}
