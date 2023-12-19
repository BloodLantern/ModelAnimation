#pragma once

#include "montage/animation_montage_command.h"

class AnimCmdWait : public AnimationMontageCommand
{
private:
	float m_Duration;

public:
	AnimCmdWait(const float duration);

	bool OnUpdate(const float deltaTime) override;
};
