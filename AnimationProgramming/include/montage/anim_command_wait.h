#pragma once

#include "montage/animation_montage_command.h"

/// <summary>
/// Waits for a set amount of time
/// </summary>
class AnimCmdWait : public AnimationMontageCommand
{
private:
	float m_Duration;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="duration">Duration to wait</param>
	AnimCmdWait(const float duration);

	bool OnUpdate(const float deltaTime) override;
};
