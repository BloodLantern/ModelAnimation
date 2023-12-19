#pragma once

#include "montage/animation_montage_command.h"

class AnimCmdCrossFade : public AnimationMontageCommand
{
private:
	size_t m_StartAnimationId;
	size_t m_EndAnimationId;
	float m_Duration;

	float m_NormalizedTimeScale;

public:
	AnimCmdCrossFade(const float duration, const size_t startAnimId, const size_t endAnimId);

	void OnBegin() override;
	bool OnUpdate(const float deltaTime) override;
};
