#pragma once

#include "skeleton.h"
#include "key_frame.h"

class Animation
{
private:
	std::string m_Name;
	size_t m_KeyCount;
	Skeleton* m_Skeleton;

	std::vector<std::vector<KeyFrame>> m_KeyFrames;
	std::vector<KeyFrame> m_LastKeyFrames;
	
	std::vector<Vector3> m_CurrentFramePositions;
	std::vector<Quaternion> m_CurrentFrameRotations;

	int m_Framerate = 30;
	float m_FrameDuration;
	
	float m_Time = 0.f;
	float m_Duration;

	float m_CrossFadeAlphaMax;
	float m_CrossFadeAlpha;
	bool m_IsCrossFadeAuto;
	Animation* m_BlendTarget;

	void UpdateTime(const float deltaTime);

public:
	size_t CurrentFrame = 0;
	bool Paused = false;
	float Speed = 1.f;

	Animation(std::string&& name, const size_t keyCount, Skeleton* skeleton);

	_NODISCARD size_t GetKeyCount() const;
	_NODISCARD size_t GetBoneCount() const;
	_NODISCARD const std::string& GetName() const;
	_NODISCARD const KeyFrame& GetKeyFrame(const size_t frame, const size_t boneIndex) const;
	_NODISCARD float GetDuration() const;

	void AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation);

	void Animate(const float deltaTime);
	void StartCrossFade(const float alpha, const bool isAuto);

	static void CrossFade(Animation& start, Animation& end, const float deltaTime);
};
