#include "gltf/animation.h"

void Animation::Load(const rapidjson::Value& value)
{
    auto&& array = value["channels"].GetArray();
    channels.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
    {
        Channel channel;
        channel.Load(array[i]);
        channels[i] = channel;
    }
    
    array = value["samplers"].GetArray();
    samplers.resize(array.Size());
    for (unsigned int i = 0; i < array.Size(); i++)
    {
        Sampler sampler;
        sampler.Load(array[i]);
        samplers[i] = sampler;
    }
}
