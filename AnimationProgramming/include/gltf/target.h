#pragma once

#include <string>

#include "rapidjson/document.h"

class Target
{
public:
    int node;
    std::string path;

    void Load(const rapidjson::Value& value);
};
