#include "montage/anim_command_play_mixed.h"
#include "montage/animation_montage.h"

AnimCmdPlayMixed::AnimCmdPlayMixed(float* alpha, const size_t startAnimId, const size_t endAnimId)
	: AnimationMontageCommand("Play mixed"), m_Alpha(alpha), m_AnimationStart(startAnimId), m_AnimationEnd(endAnimId)
{
}

bool AnimCmdPlayMixed::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	Animation& animStart = m_Montage->GetAnimation(m_AnimationStart);
	Animation& animEnd = m_Montage->GetAnimation(m_AnimationEnd);

	animStart.StartCrossFade(*m_Alpha, false);
	Animation::CrossFade(animStart, animEnd, deltaTime);

	return false;
}
