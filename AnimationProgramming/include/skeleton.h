#pragma once

#include <stdint.h>
#include "bone.h"

class Skeleton
{
	std::vector<Bone> m_Bones;

	static void DrawRecursive(Bone& bone, const Vector3& parentPos);

public:
	void Reserve(size_t nbr);
	void AddBone(const Bone& bone, size_t index);
	void AddBone(const Bone& bone);

	_NODISCARD Bone& GetRoot();
	_NODISCARD size_t GetBoneCount() const;
	_NODISCARD const Bone& GetBone(const size_t id) const;
	_NODISCARD std::vector<Bone>& GetBones();

	void SetupFamily();

	void Draw();

	void Load(const std::string& filename);
};
