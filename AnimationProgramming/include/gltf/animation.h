#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>

#include "channel.h"
#include "sampler.h"

class Animation
{
public:
    std::vector<Channel> channels;
    std::vector<Sampler> samplers;
    std::string name;
    
    void Load(const rapidjson::Value& value);
};
