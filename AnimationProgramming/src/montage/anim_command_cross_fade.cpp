#include "montage/anim_command_cross_fade.h"
#include "montage/animation_montage.h"

#include <cmath>

AnimCmdCrossFade::AnimCmdCrossFade(const float duration, const size_t startAnimId, const size_t endAnimId)
	: AnimationMontageCommand("Cross fade"), m_Duration(duration),
	  m_StartAnimationId(startAnimId), m_EndAnimationId(endAnimId)
{
}

void AnimCmdCrossFade::OnBegin()
{
	AnimationMontageCommand::OnBegin();

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	m_NormalizedTimeScale = animEnd.GetDuration() / animStart.GetDuration();
	animStart.StartCrossFade(m_Duration, true);
}

bool AnimCmdCrossFade::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	m_Time += deltaTime;

	const float t = m_Time / m_Duration;

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	animEnd.Speed = std::lerp(m_NormalizedTimeScale, 1.f, t);

	Animation::CrossFade(animStart, animEnd, deltaTime);

	if (m_Time >= m_Duration)
		return true;

	return false;
}
