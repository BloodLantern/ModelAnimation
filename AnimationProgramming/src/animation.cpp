#include "animation.h"
#include "Engine_extensions.h"

#include <iostream>

#include "maths/calc.hpp"

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

Animation::Animation(std::string&& name, const size_t keyCount, Skeleton* skeleton)
	: m_Name(std::move(name))
	, m_KeyCount(keyCount)
	, m_Skeleton(skeleton)
	, m_FrameDuration(1.f / (float) m_Framerate)
	, m_Duration((float) keyCount * m_FrameDuration)
{
	m_KeyFrames.resize(keyCount);
	m_LastKeyFrames.resize(skeleton->GetBoneCount());

	for (size_t i = 0; i < keyCount; i++)
		m_KeyFrames[i].resize(skeleton->GetBoneCount());

	m_LastKeyFrames = m_KeyFrames[0];
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

void Animation::Animate(const float deltaTime)
{
	UpdateTime(deltaTime);

	const size_t boneCount = GetBoneCount();

	float t;
	if (m_CrossFadeAlphaMax < 0)
	{
		t = std::fmodf(m_Time, m_FrameDuration) / m_FrameDuration;
		if (Speed > 0)
			t = 1 - t;
	}
	else
	{
		m_CrossFadeAlpha += deltaTime;
		t = m_CrossFadeAlpha / m_CrossFadeAlphaMax;

		if (m_CrossFadeAlpha > m_CrossFadeAlphaMax)
		{
			t = 1.f;
			m_CrossFadeAlphaMax = -1.f;
		}
	}

	std::vector<Matrix4x4> matrices(boneCount);
	std::vector<Matrix4x4> animMatrices(boneCount);
	
	const std::vector<KeyFrame>& keyFrames = m_KeyFrames[CurrentFrame];

	for (size_t i = 0; i < boneCount; i++)
	{
		const Bone& bone = m_Skeleton->GetBone(i);

		const Vector3 position = calc::Lerp(keyFrames[i].GetPosition(), m_LastKeyFrames[i].GetPosition(), t);
		const Quaternion rotation = Quaternion::Slerp(keyFrames[i].GetRotation(), m_LastKeyFrames[i].GetRotation(), t);
		const Matrix4x4 transform = Matrix4x4::TRS(position, rotation, 1.f);
		
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

	EngineExt::SetSkinningPose(matrices);
}

void Animation::StartCrossFade(const float alpha)
{
	m_CrossFadeAlphaMax = alpha;
	m_CrossFadeAlpha = 0.f;
}

void Animation::CrossFade(Animation& start, Animation& end, const float deltaTime)
{
	end.UpdateTime(deltaTime);

	start.m_LastKeyFrames = end.m_KeyFrames[end.CurrentFrame];

	start.Animate(deltaTime);
}
