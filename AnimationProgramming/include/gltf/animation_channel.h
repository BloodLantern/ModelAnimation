#pragma once

#include "channel_target.h"

#include "rapidjson/document.h"

class Channel
{
public:
    int sampler;
    Target target;

    Channel() = default;

    explicit Channel(const rapidjson::Value& value);
};
