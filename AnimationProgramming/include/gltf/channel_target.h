#pragma once

#include <string>

#include "rapidjson/document.h"

class Target
{
public:
    int node;
    std::string path;

    Target() = default;

    explicit Target(const rapidjson::Value& value);
};
