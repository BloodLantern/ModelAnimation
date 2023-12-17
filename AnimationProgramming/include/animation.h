#pragma once

#include <stdint.h>
#include "skeleton.h"

class Animation
{
private:
	struct KeyFrame;
	std::string m_Name;
	size_t m_KeyCount;
	const Skeleton& m_Skeleton;

	std::vector<std::vector<KeyFrame>> m_KeyFrames;

public:
	struct KeyFrame
	{
		Vector3 Position;
		Quaternion Rotation;

		KeyFrame()
			: Position(0.f), Rotation(0.f)
		{}

		KeyFrame(const Vector3& position, const Quaternion& rotation)
			: Position(position), Rotation(rotation)
		{}
	};

	Animation(std::string&& name, const size_t keyCount, const Skeleton& skeleton);

	_NODISCARD size_t GetKeyCount() const;
	_NODISCARD size_t GetBoneCount() const;
	_NODISCARD const std::string& GetName() const;
	_NODISCARD const KeyFrame& GetKeyFrame(const size_t frame, const size_t boneIndex) const;

	void AddKeyFrame(const size_t frame, const size_t boneIndex, const Vector3& position, const Quaternion& rotation);
};
