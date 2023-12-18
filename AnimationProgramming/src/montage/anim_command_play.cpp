#include "montage/anim_command_play.h"
#include "montage/animation_montage.h"

AnimCmdPlay::AnimCmdPlay(const float duration, const size_t animationId)
	: AnimationMontageCommand("Play"), m_Duration(duration), m_AnimationId(animationId)
{
}

void AnimCmdPlay::OnUpdate(const float deltaTime) const
{
}
