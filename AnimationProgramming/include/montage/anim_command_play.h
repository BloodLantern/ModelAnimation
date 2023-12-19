#pragma once

#include "montage/animation_montage_command.h"

class AnimCmdPlay : public AnimationMontageCommand
{
private:
	float m_Duration;
	size_t m_AnimationId;

public:
	AnimCmdPlay(const float duration, const size_t animationId);

	bool OnUpdate(const float deltaTime) override;
};