#pragma once

#include <string>

#include "rapidjson/document.h"

class Sampler
{
public:
    int input;
    std::string interpolation = "LINEAR";
    int output;

    Sampler() = default;

    explicit Sampler(const rapidjson::Value& value);
};
