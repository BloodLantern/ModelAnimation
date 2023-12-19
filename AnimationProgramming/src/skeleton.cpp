#include "skeleton.h"
#include <cassert>
#include <iostream>

#include "Engine.h"
#include "Engine_extensions.h"

void Skeleton::Reserve(const size_t nbr)
{
	m_Bones.clear();
	m_Bones.resize(nbr);
}

void Skeleton::AddBone(const Bone& bone, const size_t index)
{
	assert(index < m_Bones.size() && "Bone index out of range");
	__assume(index < m_Bones.size());

	m_Bones[index] = bone;
}

void Skeleton::AddBone(const Bone& bone)
{
	m_Bones.push_back(bone);
}

_NODISCARD Bone& Skeleton::GetRoot()
{
	assert(!m_Bones.empty() && "Skeleton has no bones");
	__assume(!m_Bones.empty());

	return m_Bones[0];
}

size_t Skeleton::GetBoneCount() const
{
	return m_Bones.size();
}

const Bone& Skeleton::GetBone(const size_t id) const
{
	return m_Bones[id];
}

void Skeleton::SetupFamily()
{
	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		const int parentId = GetSkeletonBoneParentIndex(i);

		if (parentId != -1)
		{
			m_Bones[i].SetParent(&m_Bones[parentId], parentId);

			m_Bones[parentId].AddChild(&m_Bones[i]);
		}
		else
		{
			m_Bones[i].SetParent(nullptr, -1);
		}
	}

	for (Bone& b : m_Bones)
	{
		b.ComputeMatrixes();
	}
}

void Skeleton::Draw()
{
	Bone& root = GetRoot();
	Draw_Recursive(root, Vector3(0.f));
}

void Skeleton::Draw_Recursive(Bone& bone, const Vector3& parentPos)
{
	constexpr Vector4 v(0.f, 0.f, 0.f, 1.f);
	
	const Vector4 p = bone.GetGlobalTransform() * v;
	const Vector3 position = Vector3(p.x, p.y, p.z);

	if (parentPos != Vector3(0.f))
		EngineExt::DrawLine(parentPos, position, Vector3(0.f));

	for (Bone* b : bone.GetChildren())
	{
		Draw_Recursive(*b, position);
	}
}
