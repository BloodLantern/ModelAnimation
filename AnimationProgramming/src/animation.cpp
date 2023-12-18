#include "animation.h"
#include "Engine_extensions.h"

#include <iostream>

#include "maths/calc.hpp"

static constexpr const float SAMPLE_TIME = (1 / 30.f);

Animation::Animation(std::string&& name, const size_t keyCount, const Skeleton& skeleton)
	: m_Name(std::move(name)), m_KeyCount(keyCount), m_Skeleton(skeleton)
{
	m_KeyFrames.resize(keyCount);
	m_LastKeyFrames.resize(skeleton.GetBoneCount());

	for (size_t i = 0; i < keyCount; i++)
	{
		m_KeyFrames[i].resize(skeleton.GetBoneCount());
	}

	m_LastKeyFrames = m_KeyFrames[0];

	Paused = false;
	m_Time = 0.f;
	CurrentFrame = 0;
	DeltaModulation = 1.f;
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

const KeyFrame& Animation::GetKeyFrame(const size_t frame, const size_t boneIndex) const
{
	return m_KeyFrames[frame][boneIndex];
}

void Animation::AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation)
{
	m_KeyFrames[frame][boneIndex] = KeyFrame(position, rotation);
}

void Animation::Animate(const float deltaTime)
{
	if (DeltaModulation < 0.f)
		DeltaModulation = 0.f;

	if (!Paused)
	{
		m_Time += deltaTime * DeltaModulation;
		m_Time = std::fmodf(m_Time, m_KeyCount * SAMPLE_TIME);
		
		const size_t lastFrame = CurrentFrame;
		CurrentFrame = (size_t) (m_Time / SAMPLE_TIME);

		if (lastFrame != CurrentFrame)
			m_LastKeyFrames = m_KeyFrames[lastFrame];
	}

	const size_t boneCount = GetBoneCount();

	const float t = std::fmodf(m_Time, SAMPLE_TIME) / SAMPLE_TIME;

	std::vector<Matrix4x4> matrices(boneCount);
	std::vector<Matrix4x4> animMatrices(boneCount);
	
	const std::vector<KeyFrame>& keyFrames = m_KeyFrames[CurrentFrame];

	for (size_t i = 0; i < boneCount; i++)
	{
		const Bone& bone = m_Skeleton.GetBone(i);

		const Vector3 position = calc::Lerp(m_LastKeyFrames[i].GetPosition(), keyFrames[i].GetPosition(), t);
		const Quaternion rotation = Quaternion::Slerp(m_LastKeyFrames[i].GetRotation(), keyFrames[i].GetRotation(), t);
		const Matrix4x4 transform = Matrix4x4::TRS(position, rotation, 1.f);
		
		const int parentIdx = GetSkeletonBoneParentIndex(i);
		if (parentIdx != -1)
		{
			animMatrices[i] = animMatrices[parentIdx] * bone.GetLocalTransform() * transform;
		}
		else
		{
			animMatrices[i] = bone.GetLocalTransform() * transform;
		}
		
		matrices[i] = (animMatrices[i] * bone.GetGlobalInvTransform()).Transpose();
	}

	EngineExt::SetSkinningPose(matrices);
}
