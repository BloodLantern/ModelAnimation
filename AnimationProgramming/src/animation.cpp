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

	m_IkTransforms.resize(boneCount);
	ResetIkTransforms();
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

void Animation::SetCurrentFrame(const size_t frame)
{
	m_Time = frame * m_FrameDuration;
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

float Animation::GetBlendTime(const float deltaTime)
{
	if (!m_IsCrossFadeAuto)
		return m_CrossFadeAlphaMax;
	
	m_CrossFadeAlpha += deltaTime;

	if (m_CrossFadeAlpha > m_CrossFadeAlphaMax)
	{
		m_CrossFadeAlphaMax = -1.f;
		m_BlendTarget = nullptr;
		return 1.f;
	}

	return m_CrossFadeAlpha / m_CrossFadeAlphaMax;
}

void Animation::ResetIkTransforms()
{
	for (size_t i = 0; i < m_IkTransforms.size(); i++)
		m_IkTransforms[i] = KeyFrame(Vector3(0.f), Quaternion::Identity());
}

void Animation::Animate(const float deltaTime)
{
	// Update time related varibles
	UpdateTime(deltaTime);
	
	const size_t boneCount = GetBoneCount();

	// This vector will hold the matrices that are sent to the GPU
	std::vector<Matrix4x4> matrices(boneCount);

	// This vector holds the global transform of each animated bone
	std::vector<Matrix4x4> animMatrices(boneCount);

	// Get normalized time in between frames to interpolate between 2 frames
	float t = std::fmodf(m_Time, m_FrameDuration) / m_FrameDuration;

	// Reverse the normalized time if the animation is playing in reverse
	// in which case we want to interpolate from the current frame to the last frame 
	if (Speed < 0)
		t = 1 - t;

	// Get current key frames
	const std::vector<KeyFrame>& keyFrames = m_KeyFrames[CurrentFrame];

	// Get blend target and the lerp value for the blend
	Animation* target = m_BlendTarget;
	const float tBlend = GetBlendTime(deltaTime);

	// Apply animation for each bone
	for (size_t i = 0; i < boneCount; i++)
	{
		const Bone& bone = m_Skeleton->GetBone(i);

		// Process lerp using the previous frame and the current one
		m_CurrentFramePositions[i] = calc::Lerp(m_LastKeyFrames[i].GetPosition(), keyFrames[i].GetPosition(), t);
		m_CurrentFrameRotations[i] = Quaternion::Slerp(m_LastKeyFrames[i].GetRotation(), keyFrames[i].GetRotation(), t);

		// Check for blend lerp
		if (target)
		{
			// Lerp between the current animation computed position/rotation and the target animation computed position/rotation
			m_CurrentFramePositions[i] = calc::Lerp(m_CurrentFramePositions[i], target->m_CurrentFramePositions[i], tBlend);
			m_CurrentFrameRotations[i] = Quaternion::Slerp(m_CurrentFrameRotations[i], target->m_CurrentFrameRotations[i], tBlend);
		}
		
		// Create TRS from the animation
		const Matrix4x4 transform = Matrix4x4::TRS(m_CurrentFramePositions[i], m_CurrentFrameRotations[i], 1.f);

		// Create TRS from the IK animation
		const Matrix4x4 ikTransform = Matrix4x4::TRS(m_IkTransforms[i].GetPosition(), m_IkTransforms[i].GetRotation(), 1.f);

		// Get final animation transform
		const Matrix4x4 localTransform = transform * ikTransform;

		// Apply the transform locally to the bone
		const Matrix4x4 localAnim = bone.GetLocalTransform() * localTransform;

		// Feed the animMatrices array
		const int parentIdx = bone.GetParentIndex();
		if (parentIdx != -1)
		{
			// The bone has a parent, so apply the parent global transform to it
			animMatrices[i] = animMatrices[parentIdx] * localAnim;
		}
		else
		{
			// The bone has no parent, so its local transform is the same as its local
			animMatrices[i] = localAnim;
		}
		
		// Apply the inverse to the global transform to remove the bind pose transform, also transpose the result because OpenGl
		matrices[i] = (animMatrices[i] * bone.GetGlobalInvTransform()).Transpose();
	}

	// Clear ik keyframes after the animation was processed
	ResetIkTransforms();

	// Forward matrices to the GPU
#ifndef NOENGINE
	EngineExt::SetSkinningPose(matrices);
#else
	// TODO: Apply animation without engine
#endif
}

void Animation::StartCrossFade(const float alpha, const bool isAuto, Animation* target)
{
	m_CrossFadeAlphaMax = alpha;
	m_CrossFadeAlpha = 0.f;

	m_IsCrossFadeAuto = isAuto;
	m_BlendTarget = target;
}

void Animation::ProcessIk(const Vector3& target, const size_t affectedBone)
{
	const Bone& bone = m_Skeleton->GetBone(affectedBone);
	int parentId = bone.GetParentIndex();

	{
		Vector3 forward = (target - bone.Position).Normalized();
		Vector3 right = Vector3::Cross(forward, Vector3::UnitY()).Normalized();
		Vector3 up = Vector3::Cross(right, forward);

#ifndef NOENGINE
		EngineExt::DrawLine(Vector3(250.f, 0.f, 0.f), Vector3(250.f, 0.f, 0.f) + forward * 100.f, Vector3(1.f, 0.f, 0.f));
		EngineExt::DrawLine(Vector3(250.f, 0.f, 0.f), Vector3(250.f, 0.f, 0.f) + right * 100.f, Vector3(0.f, 1.f, 0.f));
		EngineExt::DrawLine(Vector3(250.f, 0.f, 0.f), Vector3(250.f, 0.f, 0.f) + up * 100.f, Vector3(0.f, 0.f, 1.f));
#endif

		Matrix4x4 view;
		Matrix4x4::ViewMatrix(Vector3(0.f), forward, up, view);

		m_IkTransforms[affectedBone] = KeyFrame(Vector3(0.f), Quaternion::FromRotationMatrix(view));
	}

	if (parentId != -1)
	{
		ProcessIk(bone.Position, parentId);
	}
}
