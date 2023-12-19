#pragma once

#include "montage/animation_montage_command.h"

class AnimCmdPlayMixed : public AnimationMontageCommand
{
private:
	float* m_Alpha;
	size_t m_AnimationStart;
	size_t m_AnimationEnd;

public:
	AnimCmdPlayMixed(float* alpha, const size_t startAnimId, const size_t endAnimId);

	bool OnUpdate(const float deltaTime) override;
};
