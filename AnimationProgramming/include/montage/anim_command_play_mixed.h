#pragma once

#include "montage/animation_montage_command.h"

/// <summary>
/// Plays a mix between 2 animations
/// </summary>
class AnimCmdPlayMixed : public AnimationMontageCommand
{
private:
	float* m_Alpha;
	size_t m_StartAnimationId;
	size_t m_EndAnimationId;

public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="alpha">Pointer to the t value (between 0.f and 1.f)</param>
	/// <param name="startAnimId">First animation (from)</param>
	/// <param name="endAnimId">Second animation (to)</param>
	AnimCmdPlayMixed(float* alpha, const size_t startAnimId, const size_t endAnimId);

	void OnBegin() override;
	bool OnUpdate(const float deltaTime) override;
};
