#include "gltf/camera_perspective.h"

#include "utils.h"

CameraPerspective::CameraPerspective(const rapidjson::Value& value)
{
    utils::SetFromJsonSafe(VAR_AND_NAME(aspectRatio), value);
    
    utils::SetFromJson(VAR_AND_NAME(yfov), value);
    
    utils::SetFromJsonSafe(VAR_AND_NAME(zfar), value);
    
    utils::SetFromJson(VAR_AND_NAME(znear), value);
}
