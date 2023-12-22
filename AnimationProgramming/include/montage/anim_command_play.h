#pragma once

#include "montage/animation_montage_command.h"

/// <summary>
/// Plays a specified animation for a set amount of time
/// </summary>
class AnimCmdPlay : public AnimationMontageCommand
{
private:
	float m_Duration;
	size_t m_AnimationId;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="duration">How long the animation sould play, a negative value means play the animation once, fully</param>
	/// <param name="animationId">Animation id in the montage</param>
	AnimCmdPlay(const float duration, const size_t animationId);

	bool OnUpdate(const float deltaTime) override;
};
