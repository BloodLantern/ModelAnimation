#pragma once

#include "Engine.h"

#include "maths/vector3.hpp"
#include "maths/quaternion.hpp"

namespace EngineExt
{
	void GetSkeletonBoneLocalBindTransform(int boneIndex, Vector3& p, Quaternion& r);

	void DrawLine(const Vector3& p1, const Vector3& p2, const Vector3& color);
};
