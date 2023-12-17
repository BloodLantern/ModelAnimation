#pragma once

#include <stdint.h>
#include "skeleton.h"
#include "key_frame.h"

class Animation
{
private:
	std::string m_Name;
	size_t m_KeyCount;
	const Skeleton& m_Skeleton;

	std::vector<std::vector<KeyFrame>> m_KeyFrames;

public:
	Animation(std::string&& name, const size_t keyCount, const Skeleton& skeleton);

	_NODISCARD size_t GetKeyCount() const;
	_NODISCARD size_t GetBoneCount() const;
	_NODISCARD const std::string& GetName() const;
	_NODISCARD const KeyFrame& GetKeyFrame(const size_t frame, const size_t boneIndex) const;

	void AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation);

	void Animate();
};
