#include "montage/anim_command_cross_fade.h"
#include "montage/animation_montage.h"

#include <cmath>

AnimCmdCrossFade::AnimCmdCrossFade(const float duration, const size_t startAnimId, const size_t endAnimId)
	: AnimationMontageCommand("Cross fade"), m_Duration(duration),
	  m_StartAnimationId(startAnimId), m_EndAnimationId(endAnimId), m_NormalizedTimeScale(0.f)
{
}

void AnimCmdCrossFade::OnBegin()
{
	AnimationMontageCommand::OnBegin();

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	animEnd.Speed = animEnd.GetDuration() / animStart.GetDuration();
	animEnd.SetCurrentFrame(animStart.CurrentFrame * animEnd.Speed);

	animStart.StartCrossFade(m_Duration, true, &animEnd);
}

bool AnimCmdCrossFade::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	m_Time += deltaTime;

	const float t = m_Time / m_Duration;

	Animation& animStart = m_Montage->GetAnimation(m_StartAnimationId);
	Animation& animEnd = m_Montage->GetAnimation(m_EndAnimationId);

	animEnd.Animate(deltaTime);
	animStart.Animate(deltaTime);

	if (m_Time >= m_Duration)
	{
		animEnd.Speed = 1.f;
		return true;
	}

	return false;
}
