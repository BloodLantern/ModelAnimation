#include "montage/anim_command_play.h"
#include "montage/animation_montage.h"

AnimCmdPlay::AnimCmdPlay(const float duration, const size_t animationId)
	: AnimationMontageCommand("Play"), m_Duration(duration), m_AnimationId(animationId)
{
}

bool AnimCmdPlay::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	m_Time += deltaTime;

	if (m_Duration > 0.f && m_Time > m_Duration)
		return true;

	m_Montage->GetAnimation(m_AnimationId).Animate(deltaTime);

	return false;
}
