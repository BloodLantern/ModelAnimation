#include "skeleton.h"
#include <cassert>
#include <iostream>

#include "Engine.h"
#include "Engine_extensions.h"

void Skeleton::Reserve(const size_t nbr)
{
	m_Bones.clear();
	m_Bones.reserve(nbr);
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

void Skeleton::SetupFamily()
{
	for (size_t i = 0; i < m_Bones.size(); i++)
	{
		const int parentId = GetSkeletonBoneParentIndex(i);

		if (parentId == -1)
			continue;

		m_Bones[i].SetParent(&m_Bones[parentId]);

		m_Bones[parentId].AddChild(&m_Bones[i]);
	}
}

void Skeleton::Draw()
{
	Bone& root = GetRoot();
	Draw_Recursive(root, Vector3(0.f), Matrix4x4::Identity());
}

void Skeleton::Draw_Recursive(Bone& bone, const Vector3& parentPos, const Matrix4x4& parentTrs)
{
	const Matrix4x4 trs = parentTrs * bone.GetLocalTransform();
	constexpr Vector4 v(0.f, 0.f, 0.f, 1.f);
	
	const Vector4 p = trs * v;
	const Vector3 position = Vector3(p.x, p.y, p.z);

	EngineExt::DrawLine(parentPos, position, Vector3(0.f));
	for (Bone* b : bone.GetChildren())
	{
		Draw_Recursive(*b, position, trs);
	}
}
