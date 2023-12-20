#include "montage/anim_command_play_mixed.h"
#include "montage/animation_montage.h"

AnimCmdPlayMixed::AnimCmdPlayMixed(float* alpha, const size_t startAnimId, const size_t endAnimId)
	: AnimationMontageCommand("Play mixed"), m_Alpha(alpha), m_StartAnimationId(startAnimId), m_EndAnimationId(endAnimId)
{
}

void AnimCmdPlayMixed::OnBegin()
{
	AnimationMontageCommand::OnBegin();

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	animEnd.Speed = animEnd.GetDuration() / animStart.GetDuration();
}

bool AnimCmdPlayMixed::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	animStart.StartCrossFade(*m_Alpha, false);
	Animation::CrossFade(animStart, animEnd, deltaTime);

	return false;
}
