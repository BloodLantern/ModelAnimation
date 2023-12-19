#include "montage/animation_montage_command.h"

AnimationMontageCommand::AnimationMontageCommand(std::string&& displayName)
    : m_DisplayName(std::move(displayName))
{
}

void AnimationMontageCommand::OnAdded(AnimationMontage* montage)
{
    m_Montage = montage;
}

bool AnimationMontageCommand::OnUpdate(const float deltaTime)
{
    if (!m_HasStarted)
        OnBegin();

    return false;
}

void AnimationMontageCommand::OnBegin()
{
    m_HasStarted = true;

    m_Time = 0.f;
}

void AnimationMontageCommand::OnEnd()
{
    m_HasStarted = false;
}
