#include "montage/anim_command_wait.h"

AnimCmdWait::AnimCmdWait(const float duration)
	: AnimationMontageCommand("Wait"), m_Duration(duration)
{
}

bool AnimCmdWait::OnUpdate(const float deltaTime)
{
	AnimationMontageCommand::OnUpdate(deltaTime);

	m_Time += deltaTime;

	if (m_Time > m_Duration)
		return true;

	return false;
}
