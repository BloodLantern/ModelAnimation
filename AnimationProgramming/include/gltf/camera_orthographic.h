#pragma once

#include "rapidjson/document.h"

class CameraOrthographic
{
public:
    float xmag;
    float ymag;
    float zfar;
    float znear;

    CameraOrthographic() = default;

    explicit CameraOrthographic(const rapidjson::Value& value);
};
