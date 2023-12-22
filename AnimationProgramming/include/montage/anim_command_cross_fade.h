#pragma once

#include "montage/animation_montage_command.h"

/// <summary>
/// Performs a cross fade between 2 animations
/// </summary>
class AnimCmdCrossFade : public AnimationMontageCommand
{
private:
	size_t m_StartAnimationId;
	size_t m_EndAnimationId;
	float m_Duration;

	float m_NormalizedTimeScale;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="duration">Duration of the cross fade</param>
	/// <param name="startAnimId">First animation (from)</param>
	/// <param name="endAnimId">Second animation (to)</param>
	AnimCmdCrossFade(const float duration, const size_t startAnimId, const size_t endAnimId);

	void OnBegin() override;
	bool OnUpdate(const float deltaTime) override;
};
