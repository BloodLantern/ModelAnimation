#include "skeleton.h"
#include <cassert>

#include "Engine.h"

void Skeleton::Reserve(const size_t nbr)
{
	m_Bones.clear();
	m_Bones.reserve(nbr);
}

void Skeleton::AddBone(const Bone bone, const size_t index)
{
	assert(index >= 0 && index < m_Bones.size() && "Bone index out of range");
	__assume(index >= 0 && index < m_Bones.size());

	m_Bones[index] = bone;
}

void Skeleton::AddBone(const Bone bone)
{
	m_Bones.push_back(bone);
}

_NODISCARD const Bone& Skeleton::GetRoot() const
{
	assert(m_Bones.size() != 0 && "Skeleton has no bones");
	__assume(m_Bones.size() != 0);

	return m_Bones[0];
}

void Skeleton::SetupFamily()
{
	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		const size_t parentId = GetSkeletonBoneParentIndex(i);

		if (parentId == -1)
			continue;

		m_Bones[i].SetParent(&m_Bones[parentId]);

		m_Bones[parentId].AddChild(&m_Bones[i]);
	}
}
