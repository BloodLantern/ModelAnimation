#pragma once

#include "target.h"

#include "rapidjson/document.h"

class Channel
{
public:
    int sampler;
    Target target;
    
    void Load(const rapidjson::Value& value);
};
