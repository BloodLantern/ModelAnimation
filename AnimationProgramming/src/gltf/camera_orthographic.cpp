#include "gltf/camera_orthographic.h"

#include "utils.h"

CameraOrthographic::CameraOrthographic(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(xmag), value);
    
    utils::SetFromJson(VAR_AND_NAME(ymag), value);
    
    utils::SetFromJson(VAR_AND_NAME(zfar), value);
    
    utils::SetFromJson(VAR_AND_NAME(znear), value);
}
