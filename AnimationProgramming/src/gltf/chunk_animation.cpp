#include "..\..\include\gltf\chunk_animation.h"

void ChunkAnimation::Load(const rapidjson::Value& value)
{
    const auto& channelArray = value["channels"].GetArray();
    channels.resize(channelArray.Size());
    for (unsigned int i = 0; i < channelArray.Size(); i++)
    {
        Channel channel;
        channel.Load(channelArray[i]);
        channels[i] = channel;
    }
    
    const auto& samplerArray = value["samplers"].GetArray();
    samplers.resize(samplerArray.Size());
    for (unsigned int i = 0; i < samplerArray.Size(); i++)
    {
        Sampler sampler;
        sampler.Load(samplerArray[i]);
        samplers[i] = sampler;
    }
}
