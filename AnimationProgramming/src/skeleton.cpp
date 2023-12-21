#include "skeleton.h"
#include "Engine.h"
#include "Engine_extensions.h"
#include <cassert>
#include <fstream>
#include <iostream>

#include "ImGui/imgui_impl_opengl3_loader.h"

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
#ifndef NOENGINE
		const int parentId = GetSkeletonBoneParentIndex(i);
#else
		const int parentId = m_Bones[i].GetParentIndex();
#endif

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
		b.ComputeMatrices();
	}
}

void Skeleton::Draw()
{
	Bone& root = GetRoot();
	DrawRecursive(root, Vector3(0.f));
}

void Skeleton::Load(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	int boneCount;
	file.read(reinterpret_cast<char*>(&boneCount), sizeof(boneCount));
	m_Bones.resize(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		unsigned int stringLength;
		file.read(reinterpret_cast<char*>(&stringLength), sizeof(stringLength));
		std::string boneName;
		boneName.resize(stringLength);
		file.read(boneName.data(), stringLength);

		int boneIndex, parentIndex;
		file.read(reinterpret_cast<char*>(&boneIndex), sizeof(boneIndex));
		file.read(reinterpret_cast<char*>(&parentIndex), sizeof(parentIndex));

		m_Bones[i] = Bone(boneName, boneIndex, parentIndex);
	}

	for (Bone& bone : m_Bones)
	{
		file.read(reinterpret_cast<char*>(&bone.Position), sizeof(bone.Position));
		file.read(reinterpret_cast<char*>(&bone.Rotation), sizeof(bone.Rotation));
		// Skip the bone scale as we don't need it
		file.seekg(sizeof(vec3), std::ios_base::cur);
	}
}

std::vector<Bone>& Skeleton::GetBones()
{
	return m_Bones;
}

void Skeleton::DrawRecursive(Bone& bone, const Vector3& parentPos)
{
	constexpr Vector4 v(0.f, 0.f, 0.f, 1.f);
	
	const Vector4 p = bone.GetGlobalTransform() * v;
	const Vector3 position = Vector3(p.x, p.y, p.z);

	if (parentPos != Vector3(0.f))
#ifndef NOENGINE
		EngineExt::DrawLine(parentPos, position, Vector3(0.f));
#else
			; // TODO: Draw line without engine
#endif

	for (Bone* b : bone.GetChildren())
	{
		DrawRecursive(*b, position);
	}
}
