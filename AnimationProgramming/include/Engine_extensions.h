#pragma once

#include "Engine.h"

#include "maths/vector3.hpp"
#include "maths/quaternion.hpp"

#include "animation.h"

namespace EngineExt
{
	void GetSkeletonBoneLocalBindTransform(int boneIndex, Vector3& p, Quaternion& r);

	void DrawLine(const Vector3& p1, const Vector3& p2, const Vector3& color);

	void GetAnimLocalBoneTransform(const Animation& anim, int boneIndex, int keyFrameIndex, Vector3& p, Quaternion& r);

	void SetSkinningPose(const std::vector<Matrix4x4>& matrices);
};
