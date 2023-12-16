#pragma once

#include <stdint.h>
#include "bone.h"

class Skeleton
{
private:
	std::vector<Bone> m_Bones;

public:
	void Reserve(const size_t nbr);
	void AddBone(const Bone bone, const size_t index);
	void AddBone(const Bone bone);

	_NODISCARD Bone& GetRoot();

	void SetupFamily();
};
