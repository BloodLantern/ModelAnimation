#pragma once

#include "rapidjson/document.h"

class CameraPerspective
{
public:
    float aspectRatio;
    float yfov;
    float zfar;
    float znear;

    CameraPerspective() = default;

    explicit CameraPerspective(const rapidjson::Value& value);
};
