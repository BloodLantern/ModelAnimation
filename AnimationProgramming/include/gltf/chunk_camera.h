#pragma once

#include <string>

#include "camera_orthographic.h"
#include "camera_perspective.h"
#include "rapidjson/document.h"

class ChunkCamera
{
public:
    CameraOrthographic orthographic;
    CameraPerspective perspective;
    std::string type;
    std::string name;

    ChunkCamera() = default;

    explicit ChunkCamera(const rapidjson::Value& value);
};
