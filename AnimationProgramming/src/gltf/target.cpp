#include "gltf/target.h"

void Target::Load(const rapidjson::Value& value)
{
    node = value["node"].GetInt();

    path = value["path"].GetString();
}
