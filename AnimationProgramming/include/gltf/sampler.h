#pragma once

#include <string>

#include "rapidjson/document.h"

class Sampler
{
public:
    int input;
    std::string interpolation = "LINEAR";
    int output;
    
    void Load(const rapidjson::Value& value);
};
