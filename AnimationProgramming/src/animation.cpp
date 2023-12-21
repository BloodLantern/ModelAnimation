#include "animation.h"
#include "Engine_extensions.h"

#include <iostream>

#include "maths/calc.hpp"

Animation::Animation(std::string&& name, const size_t keyCount, Skeleton* skeleton)
	: m_Name(std::move(name))
	, m_KeyCount(keyCount)
	, m_Skeleton(skeleton)
	, m_FrameDuration(1.f / (float) m_Framerate)
	, m_Duration((float) keyCount * m_FrameDuration)
{
	m_KeyFrames.resize(keyCount);

	const size_t boneCount = skeleton->GetBoneCount();

	for (size_t i = 0; i < keyCount; i++)
		m_KeyFrames[i].resize(boneCount);

	m_LastKeyFrames = m_KeyFrames[0];

	m_CurrentFramePositions.resize(boneCount);
	m_CurrentFrameRotations.resize(boneCount);
}

size_t Animation::GetKeyCount() const
{
	return m_KeyCount;
}

size_t Animation::GetBoneCount() const
{
	return m_Skeleton->GetBoneCount();
}

const std::string& Animation::GetName() const
{
	return m_Name;
}

const KeyFrame& Animation::GetKeyFrame(const size_t frame, const size_t boneIndex) const
{
	return m_KeyFrames[frame][boneIndex];
}

float Animation::GetDuration() const
{
	return m_Duration;
}

void Animation::AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation)
{
	m_KeyFrames[frame][boneIndex] = KeyFrame(position, rotation);
}

void Animation::UpdateTime(const float deltaTime)
{
	if (Paused)
		return;

	m_Time += deltaTime * Speed;
	m_Time = std::fmodf(m_Time, m_Duration);

	const size_t lastFrame = CurrentFrame;
	CurrentFrame = (size_t)(m_Time / m_FrameDuration);

	if (Speed >= 0.f)
	{
		m_Time = std::fmodf(m_Time, m_Duration);
	}
	else
	{
		if (m_Time <= 0)
			m_Time = m_Duration - m_FrameDuration;
	}

	if (lastFrame != CurrentFrame)
		m_LastKeyFrames = m_KeyFrames[lastFrame];
}

void Animation::Animate(const float deltaTime)
{
	UpdateTime(deltaTime);
	
	const size_t boneCount = GetBoneCount();

	float t = std::fmodf(m_Time, m_FrameDuration) / m_FrameDuration;
	if (Speed > 0)
		t = 1 - t;

	std::vector<Matrix4x4> matrices(boneCount);
	std::vector<Matrix4x4> animMatrices(boneCount);
	
	const std::vector<KeyFrame>& keyFrames = m_KeyFrames[CurrentFrame];

	for (size_t i = 0; i < boneCount; i++)
	{
		const Bone& bone = m_Skeleton->GetBone(i);

		m_CurrentFramePositions[i] = calc::Lerp(keyFrames[i].GetPosition(), m_LastKeyFrames[i].GetPosition(), t);
		m_CurrentFrameRotations[i] = Quaternion::Slerp(keyFrames[i].GetRotation(), m_LastKeyFrames[i].GetRotation(), t);

		if (m_BlendTarget)
		{
			m_CurrentFramePositions[i] = calc::Lerp(m_CurrentFramePositions[i], m_BlendTarget->m_CurrentFramePositions[i], m_CrossFadeAlphaMax);
			m_CurrentFrameRotations[i] = Quaternion::Slerp(m_CurrentFrameRotations[i], m_BlendTarget->m_CurrentFrameRotations[i], m_CrossFadeAlphaMax);
		}
		
		const Matrix4x4 transform = Matrix4x4::TRS(m_CurrentFramePositions[i], m_CurrentFrameRotations[i], 1.f);

		const int parentIdx = bone.GetParentIndex();
		const Matrix4x4 localAnim = bone.GetLocalTransform() * transform;
		if (parentIdx != -1)
		{
			animMatrices[i] = animMatrices[parentIdx] * localAnim;
		}
		else
		{
			animMatrices[i] = localAnim;
		}
		
		matrices[i] = (animMatrices[i] * bone.GetGlobalInvTransform()).Transpose();
	}

#ifndef NOENGINE
	EngineExt::SetSkinningPose(matrices);
#else
	// TODO: Apply animation without engine
#endif
}

void Animation::StartCrossFade(const float alpha, const bool isAuto)
{
	m_CrossFadeAlphaMax = alpha;
	m_CrossFadeAlpha = 0.f;

	m_IsCrossFadeAuto = isAuto;
}

void Animation::CrossFade(Animation& start, Animation& end, const float deltaTime)
{
	start.m_BlendTarget = &end;
	end.Animate(deltaTime);
	start.Animate(deltaTime);
}
