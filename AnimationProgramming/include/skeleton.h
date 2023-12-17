#pragma once

#include <stdint.h>
#include "bone.h"

class Skeleton
{
private:
	std::vector<Bone> m_Bones;

	void Draw_Recursive(Bone& bone, const Vector3& parentPos, const Matrix4x4& parentTrs);

public:
	void Reserve(const size_t nbr);
	void AddBone(const Bone bone, const size_t index);
	void AddBone(const Bone bone);

	_NODISCARD Bone& GetRoot();
	_NODISCARD const size_t GetBoneCount() const;

	void SetupFamily();

	void Draw();
};
