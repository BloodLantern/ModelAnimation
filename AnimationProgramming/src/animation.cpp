#include "animation.h"

Animation::Animation(const std::string& name, const size_t keyCount, const Skeleton& skeleton)
	: m_Name(name), m_KeyCount(keyCount), m_Skeleton(skeleton)
{
	m_KeyFrames.resize(keyCount);

	for (size_t i = 0; i < keyCount; i++)
	{
		m_KeyFrames[i].resize(skeleton.GetBoneCount());
	}
}

size_t Animation::GetKeyCount() const
{
	return m_KeyCount;
}

size_t Animation::GetBoneCount() const
{
	return m_Skeleton.GetBoneCount();
}

const std::string& Animation::GetName() const
{
	return m_Name;
}

const Animation::KeyFrame& Animation::GetKeyFrame(const size_t frame, const size_t boneIndex) const
{
	return m_KeyFrames[frame][boneIndex];
}

void Animation::AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation)
{
	m_KeyFrames[frame][boneIndex] = KeyFrame(position, rotation);
}
