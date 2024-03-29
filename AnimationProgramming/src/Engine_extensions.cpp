#ifndef NOENGINE
#include "Engine_extensions.h"

void EngineExt::GetSkeletonBoneLocalBindTransform(const int boneIndex, Vector3& p, Quaternion& r)
{
	::GetSkeletonBoneLocalBindTransform(boneIndex, p.x, p.y, p.z, r.real, r.imaginary.x, r.imaginary.y, r.imaginary.z);
}

void EngineExt::DrawLine(const Vector3& p1, const Vector3& p2, const Vector3& color)
{
	::DrawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, color.x, color.y, color.z);
}

void EngineExt::GetAnimLocalBoneTransform(const Animation& anim, const int boneIndex, const int keyFrameIndex, Vector3& p, Quaternion& r)
{
	::GetAnimLocalBoneTransform(anim.GetName().c_str(), boneIndex, keyFrameIndex, p.x, p.y, p.z, r.real, r.imaginary.x, r.imaginary.y, r.imaginary.z);
}

void EngineExt::SetSkinningPose(const std::vector<Matrix4x4>& matrices)
{
	::SetSkinningPose(&matrices[0][0].x, matrices.size());
}
#endif
