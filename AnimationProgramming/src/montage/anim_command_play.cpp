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

	Animation& anim = m_Montage->GetAnimation(m_AnimationId);
	anim.StartCrossFade(-1.f, false, nullptr);

	if (m_Duration > 0.f)
	{
		if (m_Time > m_Duration)
			return true;
	}
	else
	{
		/*
		if (m_Time > anim.GetDuration())
			return true;
		*/
	}

	// anim.ProcessIk(Vector3(0.f, std::sin(m_Time), std::cos(m_Time)), 8);
	anim.Animate(deltaTime);

	return false;
}
