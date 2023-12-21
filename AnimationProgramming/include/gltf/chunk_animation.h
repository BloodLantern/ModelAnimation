#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>

#include "animation_channel.h"
#include "animation_sampler.h"

class ChunkAnimation
{
public:
    std::vector<Channel> channels;
    std::vector<Sampler> samplers;
    std::string name;

    ChunkAnimation() = default;

    explicit ChunkAnimation(const rapidjson::Value& value);
};
