#pragma once

#include <stdint.h>
#include "bone.h"

class Skeleton
{
private:
	std::vector<Bone> m_Bones;

	void Draw_Recursive(Bone& bone, const Vector3& parentPos);

public:
	void Reserve(size_t nbr);
	void AddBone(const Bone& bone, size_t index);
	void AddBone(const Bone& bone);

	_NODISCARD Bone& GetRoot();
	_NODISCARD size_t GetBoneCount() const;

	void SetupFamily();

	void Draw();
};
