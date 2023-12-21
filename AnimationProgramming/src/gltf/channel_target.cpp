#include "gltf/channel_target.h"

#include "utils.h"

Target::Target(const rapidjson::Value& value)
{
    utils::SetFromJson(VAR_AND_NAME(node), value);
    
    utils::SetFromJson(VAR_AND_NAME(path), value);
}
